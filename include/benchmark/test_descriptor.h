#ifndef BENCHMARK_TEST_DESCRIPTOR_H_
#define BENCHMARK_TEST_DESCRIPTOR_H_

#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <benchmark/test.h>
#include <benchmark/test_factory.h>
#include <iostream>

namespace benchmark {

class TestParameterDescriptor {
public:
    TestParameterDescriptor(const std::string &dec, const std::string &val)
    :Declaration(dec),
     Value(val)
    {

    }
public:
    std::string   Declaration;
    std::string   Value;
};

class TestParametersDescriptor {
private:
    enum QuotingState {
        /// Unquoted.
        Unquoted,


        /// Single quoted.
        SingleQuoted,


        /// Double quoted.
        DoubleQuoted
    };

    inline static std::string trimmedString(const char* start,
                                                const char* end)
    {
        while (start < end) {
            if ((*start == ' ') ||
                (*start == '\r') ||
                (*start == '\n') ||
                (*start == '\t')) {
                ++start;
            } else {
                break;
            }
        }

        while (end > start) {
            const char c = *(end - 1);

            if ((c != ' ') &&
                (c != '\r') &&
                (c != '\n') &&
                (c != '\t')) {
                break;
            }
            --end;
        }

        return std::string(start, std::string::size_type(end - start));
    }

    static std::vector<std::string>
        parseCommaSeparated(const char* separated)
    {
        std::vector<std::string> result;

        if (*separated) {
            ++separated;
        }
        while ((*separated) && (*separated != ')')) {
            std::size_t escapeCounter = 0;
            const char* start = separated;
            QuotingState state = Unquoted;
            bool escaped = false;

            while (*separated) {
                const char c = *separated++;

                if (state == Unquoted) {
                    if ((c == '"') || (c == '\'')) {
                        state = (c == '"' ? DoubleQuoted : SingleQuoted);
                        escaped = false;
                    } else if ((c == '<') ||
                                (c == '(') ||
                                (c == '[') ||
                                (c == '{')) {
                        ++escapeCounter;
                    } else if ((escapeCounter) &&
                                ((c == '>') ||
                                (c == ')') ||
                                (c == ']') ||
                                (c == '}'))) {
                        --escapeCounter;
                    } else if ((!escapeCounter) &&
                                ((c == ',') || (c == ')'))) {
                        result.push_back(trimmedString(start,
                                                        separated - 1));
                        break;
                    }
                } else {
                    if (escaped) {
                        escaped = false;
                    } else if (c == '\\') {
                        escaped = true;
                    } else if (c == (state == DoubleQuoted ? '"' : '\'')) {
                        state = Unquoted;
                    }
                }
            }
        }
        return result;
    }

    TestParameterDescriptor parseDescriptor(const std::string& raw)
    {
        const char* position = raw.c_str();

        // Split the declaration into its declaration and its default
        // type.
        const char* equalPosition = NULL;
        std::size_t escapeCounter = 0;
        QuotingState state = Unquoted;
        bool escaped = false;

        while (*position) {
            const char c = *position++;

            if (state == Unquoted) {
                if ((c == '"') || (c == '\'')) {
                    state = (c == '"' ? DoubleQuoted : SingleQuoted);
                    escaped = false;
                } else if ((c == '<') ||
                            (c == '(') ||
                            (c == '[') ||
                            (c == '{')) {
                    ++escapeCounter;
                } else if ((escapeCounter) &&
                            ((c == '>') ||
                            (c == ')') ||
                            (c == ']') ||
                            (c == '}'))){
                    --escapeCounter;
                } else if ((!escapeCounter) &&
                            (c == '=')) {
                    equalPosition = position;
                    break;
                }
            } else {
                if (escaped)
                    escaped = false;
                else if (c == '\\')
                    escaped = true;
                else if (c == (state == DoubleQuoted ? '"' : '\''))
                    state = Unquoted;
            }
        }

        // Construct the parameter descriptor.
        if (equalPosition) {
            const char* start = raw.c_str();
            const char* end = start + raw.length();

            return TestParameterDescriptor(
                std::string(trimmedString(start,
                                            equalPosition - 1)),
                std::string(trimmedString(equalPosition,
                                            end))
            );
        } else {
            return TestParameterDescriptor(raw, std::string());
        }
    }
   

public:
    TestParametersDescriptor()
    {

    }


    TestParametersDescriptor(const char* rawDeclarations,
                                const char* rawValues)
    {
        // parse the declarations.
        std::vector<std::string> declarations =
            parseCommaSeparated(rawDeclarations);

        for (std::vector<std::string>::const_iterator it =
                    declarations.begin();
                it != declarations.end();
                ++it)
            _parameters.push_back(parseDescriptor(*it));

        // parse the values.
        std::vector<std::string> values = parseCommaSeparated(rawValues);

        std::size_t straightValues = _parameters.size() > values.size() ? values.size() :_parameters.size();
        std::size_t variadicValues = 0;

        if (values.size() > _parameters.size()) {
            if (straightValues > 0) {
                --straightValues;
            }
            variadicValues = values.size() - _parameters.size() + 1;
        }

        for (std::size_t i = 0; i < straightValues; ++i) {
            _parameters[i].Value = values[i];
        }

        if (variadicValues) {
            std::stringstream variadic;

            for (std::size_t i = 0; i < variadicValues; ++i) {
                if (i) {
                    variadic << ", ";
                }
                variadic << values[straightValues + i];
            }

            _parameters[_parameters.size() - 1].Value = variadic.str();
        }
    }


    inline const std::vector<TestParameterDescriptor>& Parameters() const
    {
        return _parameters;
    }
 private:
        std::vector<TestParameterDescriptor> _parameters;

};

class TestDescriptor {
public:
 
    TestDescriptor(const char* fixtureName,
                    const char* testName,
                    std::size_t runs,
                    std::size_t iterations,
                    TestFactory* testFactory,
                    TestParametersDescriptor parameters,
                    bool isDisabled = false)
        :   FixtureName(fixtureName),
            TestName(testName),
            CanonicalName(std::string(fixtureName) + "." + testName),
            Runs(runs),
            Iterations(iterations),
            Factory(testFactory),
            Parameters(parameters),
            IsDisabled(isDisabled)
    {
    }


    /// Dispose of a test descriptor.
    ~TestDescriptor()
    {
        delete this->Factory;
    }


    /// Fixture name.
    std::string FixtureName;


    /// Test name.
    std::string TestName;


    /// Canonical name.

    /// As: <FixtureName>.<TestName>.
    std::string CanonicalName;


    /// Test runs.
    std::size_t Runs;


    /// Iterations per test run.
    std::size_t Iterations;


    /// Test factory.
    TestFactory* Factory;


    /// Parameters for parametrized tests
    TestParametersDescriptor Parameters;


    /// Disabled.
    bool IsDisabled;
};

}
#endif
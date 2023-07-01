#ifndef VS_RESULT_H
#define VS_RESULT_H

#include <utility>
#include <variant>
#include <optional>
#include <string>

namespace utils {

template<typename T, typename E = std::string>
class Result final {
public:
    using ResultType = T;
    using ErrorType = E;

    Result(const Result& ) = default;
    Result& operator=(const Result&) = default;
    Result(Result&&) noexcept = default;
    Result& operator=(Result&&) noexcept = default;

    template<typename R>
    static Result onOk(R&& result) {
        return Result(std::forward<R>(result));
    }

    template<typename R>
    static Result onError(R&& error) {
        return Result(std::forward<R>(error));
    }

    operator bool() const {
        return isOk();
    }

    bool isOk() const {
        return std::holds_alternative<ResultType>(m_data);
    }

    bool isError() const {
        return !isOk();
    }

    ResultType& operator*() {
        return value();
    }

    const ResultType& operator*() const {
        return value();
    }

    ResultType* operator->() {
        return &(value());
    }

    const ResultType* const operator->() const {
        return &(value());
    }

    ResultType& value() {
        return std::get<ResultType>(m_data);
    }

    const ResultType& value() const {
        return std::get<ResultType>(m_data);
    }

    ErrorType& error() {
        return std::get<ErrorType>(m_data);
    }

    const ErrorType& error() const {
        return std::get<ErrorType>(m_data);
    }

private:
    template<typename R>
    explicit Result(R&& r):
    m_data(std::forward<R>(r))
    {}

    std::variant<ResultType, ErrorType> m_data;
};

template<typename E>
class Result<void, E> {
public:
    using ErrorType = E;

    template<typename R>
    static Result onError(R&& error) {
        return Result(std::forward<R>(error));
    }

    static Result onOk() {
        return Result();
    }

    operator bool() const {
        return isOk();
    }

    bool isError() const {
        return m_data.has_value();
    }

    bool isOk() const {
        return !isError();
    }

    ErrorType& error() {
        return m_data.value();
    }

    const ErrorType& error() const {
        return m_data.value();
    }

private:
    template<typename R>
    explicit Result(R&& error):
    m_data(std::forward<R>(error))
    {}

    explicit Result():
    m_data(std::nullopt)
    {}

    std::optional<ErrorType> m_data;
};

} // namespace utils

#endif //! VS_RESULT_H

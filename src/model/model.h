#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <cmath>
#include <stack>
#include <array>
#include <ctime>
#include <vector>
#include <iostream>

namespace s21 {
  class model {
    public:
      model() {}
      ~model() {}
      
      bool get_error() const noexcept;
      double get_res() const noexcept;
      std::vector<double> get_cred() const noexcept;
      std::array<double, 4> get_depos() const noexcept;

      void set_x(const double& num) noexcept;
      void set_cap(const bool &cap) noexcept;
      void set_rad(const bool& graph) noexcept;
      void set_sum(const double &sum) noexcept;
      void set_tax(const double &tax) noexcept;
      void set_term(const double &term) noexcept;
      void set_period(const double &period) noexcept;
      void set_percent(const double &percent) noexcept;

      void GetNums() noexcept;
      void AnnuCred() noexcept;
      int FormatTime() noexcept;
      void DifferCred() noexcept;
      void ClearCredit() noexcept;
      void ClearOutput() noexcept;
      void Calculations() noexcept;
      void PopFunctions() noexcept;
      void DoCalculations() noexcept;
      void GetNums(double &x) noexcept;
      bool ConvertNums(const size_t &i) noexcept;
      void PushFunctions(size_t &index) noexcept;
      void GetNums(double &x, double &y) noexcept;
      void InsertNumOutput(size_t &index) noexcept;
      void Notation(const std::string &str) noexcept;
      void PushLogic(const std::string &str) noexcept;
      std::array<double, 4> s21::model::Deposit() noexcept;
      double AddSum(const double &sum, const int &time) const noexcept;

    private:
      int pos_ = 0;
      int option_ = 0;
      std::string str_;
      std::string func_;
      bool is_cap_ = false;
      bool is_rad_ = false;
      bool is_error_ = false;
      double sum_ = 0.0;
      double tax_ = 0.0;
      double term_ = 0.0;
      double result_ = 0.0;
      double period_ = 0.0;
      double percent_ = 0.0;
      double x_value_ = 0.0;
      double overpay_ = 0.0;
      double month_pay_ = 0.0;
      double f_payment_ = 0.0;
      double l_payment_ = 0.0;
      double result_sum_ = 0.0;
      double x_ = 0.0, y_ = 0.0;
      const size_t array_size_ = 11;
      std::vector<double> cred_arr_;
      std::array<double, 4> depos_arr_;
      std::stack<double> num_buffer_;
      std::stack<std::string> stack_;
      std::array<std::string, 256> output_;
      std::string functions_[11]{"cos", "sin", "tan", "acos", "asin", "atan", "ln", "log", "sqrt", "abs", "^"};
  };
}

#endif // SRC_MODEL_MODEL_H_

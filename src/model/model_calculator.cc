#include "model_calculator.h"

namespace s21 {
/*
  ================================= ACCESSORS =================================
*/
double ModelCalculator::get_res() const noexcept { return result_; }
bool ModelCalculator::get_error() const noexcept { return is_error_; }
/*
  ================================= ACCESSORS =================================
*/

/*
  ================================= MUTATORS ==================================
*/
void ModelCalculator::set_x(const double &num) noexcept { x_value_ = num; }
void ModelCalculator::set_rad(const bool &graph) noexcept { is_rad_ = graph; }
/*
  ================================= MUTATORS ==================================
*/

/*
  ============================ C A L C U L A T O R ============================
*/

void ModelCalculator::Notation(const std::string &str) noexcept {
  str_ = str;
  for (size_t i = 0; i < str_.size() && !is_error_; i++) {
    InsertNumOutput(i);
    switch (str_[i]) {
      case 'c':
      case 's':
      case 't':
      case 'a':
      case 'l':
      case '^':
      case '(':
        PushFunctions(i, 1);
        break;
      case 'm':
      case '*':
      case '/':
      case '+':
      case '-':
        PushFunctions(i, 2);
        break;
      case ')':
        PopFunctions(1);
        break;
      case '=':
        PopFunctions(2);
        break;
    }
  }
}

void ModelCalculator::InsertNumOutput(size_t &index) noexcept {
  if (isdigit(str_[index]) || str_[index] == 'x' || str_[index] == 'P') {
    std::string str_t;
    bool is_negative_ = false;
    if (str_[index - 1] == '-' && str_[index - 2] == '(')
      is_negative_ = true;
    while (isdigit(str_[index]) || str_[index] == '.' || str_[index] == 'x' || str_[index] == 'P' || str_[index] == 'i') {
      str_t += str_[index++];
    }
    output_[pos_++] = is_negative_ ? str_t + "-" : str_t;
  }
}

void ModelCalculator::PushFunctions(size_t &index, const int &opt) noexcept {
  if (opt == 1) {
    if (str_[index] == '^') {
      stack_.push("^");
    } else {
      std::string buffer_;
      while (str_[index] != '(')
        buffer_ += str_[index++];
      if (buffer_.size())
        stack_.push(buffer_);
      stack_.push("(");
    }
  } else if (opt == 2) {
    if (str_[index] == 'm') {
      PushLogic("mod");
      index += 2;
    } else if (str_[index] == '*') {
      PushLogic("*");
    } else if (str_[index] == '/') {
      PushLogic("/");
    } else if (str_[index] == '+') {
      PushLogic("+");
    } else if (str_[index] == '-') {
      if (str_[index - 1] == '(' && !isdigit(str_[index + 1]) &&
          str_[index + 1] != 'P' && str_[index + 1] != 'x')
        PushLogic("!");
      else if (str_[index - 1] != '(')
        PushLogic("-");
    }
  }
}

void ModelCalculator::PushLogic(const std::string &str) noexcept {
  if (str == "mod" || str == "*" || str == "/") {
    while (!stack_.empty() && (stack_.top() == "mod" || stack_.top() == "*"
    || stack_.top() == "/" || stack_.top() == "^" || stack_.top() == "!")) {
      output_[pos_++] = stack_.top();
      stack_.pop();
    }
  } else if (str == "+" || str == "-") {
    while (!stack_.empty() && (stack_.top() == "mod" || stack_.top() == "*"
    || stack_.top() == "/" || stack_.top() == "+" || stack_.top() == "-"
    || stack_.top() == "^" || stack_.top() == "!")) {
      output_[pos_++] = stack_.top();
      stack_.pop();
    }
  }
  stack_.push(str);
}

void ModelCalculator::PopFunctions(const int &opt) noexcept {
  if (opt == 1) {
    while (!stack_.empty() && stack_.top() != "(") {
      output_[pos_++] = stack_.top();
      stack_.pop();
    }
    if (stack_.empty()) {
      is_error_ = true;
    } else {
      stack_.pop();
      while (!stack_.empty()) {
        auto it_ = func_map_.find(stack_.top());
        if ((it_ != func_map_.end() || stack_.top() == "^")) {
          output_[pos_++] = stack_.top();
          stack_.pop();
        } else {
          break;
        }
      }
    }
  } else if (opt == 2) {
    while (!stack_.empty() && !is_error_) {
      if (stack_.top() == "(") {
        is_error_ = true;
      } else {
        output_[pos_++] = stack_.top();
        stack_.pop();
      }
    }
  }
}

void ModelCalculator::Calculations() noexcept {
  for (int i = 0; i < pos_; i++) {
    if (!ConvertNums(i)) {
      switch (output_[i].front()) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case 'm':
          GetNums(2);
          break;
        case 'c':
        case 's':
        case 't':
        case 'a':
          GetNums(output_[i] == "sqrt" || output_[i] == "abs" ? 1 : 3);
          break;
        case 'l':
          GetNums(1);
          break;
        case '!':
          GetNums(4);
          break;
      }
      auto o_it_ = oper_map_.find(output_[i]);
      auto f_it_ = func_map_.find(output_[i]);
      if (o_it_ != oper_map_.end())
        num_buffer_.push(o_it_->second(y_, x_));
      if (f_it_ != func_map_.end())
        num_buffer_.push(f_it_->second(x_));
    }
  }
  result_ = num_buffer_.top();
  num_buffer_.pop();
}

bool ModelCalculator::ConvertNums(const size_t &i) noexcept {
  double num_ = 0.0;
  if (isdigit(output_[i].front()) || output_[i].front() == 'x' || output_[i].front() == 'P') {
    if (output_[i].front() == 'x')
      num_ = x_value_;
    else if (output_[i].front() == 'P')
      num_ = M_PI;
    else
      num_ = std::stod(output_[i]);
    if (output_[i].back() == '-')
      num_ = -num_;
    num_buffer_.push(num_);
    return true;
  }
  return false;
}

void ModelCalculator::GetNums(const int &opt) noexcept {
  if (opt == 1) {
    x_ = num_buffer_.top();
    num_buffer_.pop();
  } else if (opt == 2) {
    x_ = num_buffer_.top();
    num_buffer_.pop();
    y_ = num_buffer_.top();
    num_buffer_.pop();
  } else if (opt == 3) {
    x_ = num_buffer_.top();
    num_buffer_.pop();
    x_ = is_rad_ ? x_ : x_ * M_PI / 180;
  } else if (opt == 4) {
    const double n_num_ = num_buffer_.top();
    num_buffer_.pop();
    num_buffer_.push(-n_num_);
  }
}

void ModelCalculator::ClearOutput() noexcept { pos_ = 0; }

/*
  ============================ C A L C U L A T O R ============================
*/
}  // namespace s21

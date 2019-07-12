#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "jsc/jsc.hpp"

int main(int argc, const char* argv[]) {
  auto ctx = jsc::context{};
  const auto result1 = ctx.eval_script("1 + 2 + 3").to_number();
  if (ctx.ok()) {
    std::cout << result1 << std::endl;
  }

  ctx.clear_exception();
  ctx.root()["world"] = 10;
  ctx.root()["x"] = ctx.obj();
  ctx.root()["x"].get().to_object()[12] = 30;
  const auto result2 = ctx.eval_script("1 + 2 + world + x[12]").to_number();
  if (ctx.ok()) {
    std::cout << result2 << std::endl;
  }

  ctx.clear_exception();
  const auto result3 =
      ctx.eval_script("x => x**2").to_object().call(10).to_number();
  if (ctx.ok()) {
    std::cout << result3 << std::endl;
  }

  ctx.clear_exception();
  ctx.root()["run"] =
      ctx.callable([](auto ctx, auto this_object, auto args, auto exception) {
        if (args.size() == 0) {
          *exception = ctx.error("To few arguments!");
        } else if (args.size() == 1) {
          return ctx.val(args[0].to_number() + 10);
        } else {
          *exception = ctx.error("To many arguments!");
        }
        return ctx.undefined();
      });
  const auto result4 = ctx.eval_script("run(15)").to_number();
  if (ctx.ok()) {
    std::cout << result4 << std::endl;
  }

  ctx.clear_exception();
  ctx.eval_script("run(15, 16)", "error.js");
  const auto result5 =
      ctx.get_exception().to_object()["stack"].get().to_string();
  std::cout << result5 << std::endl;

  ctx.clear_exception();
  std::ifstream input{"../test/sudoku_v1.js"};
  std::stringstream buffer;
  buffer << input.rdbuf();
  std::cout << buffer.str().length() << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  ctx.eval_script(buffer.str(), "sudoku_v1.js");
  if (!ctx.ok()) {
    std::cout << "Timed execution: exception!" << std::endl;
  } else {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << std::endl;
  }

  return 0;
}

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include <jsc/jsc.hpp>

int main() {
  jsc::context ctx;
  const auto result1 = ctx.eval_script("1 + 2 + 3").to_number();
  if (ctx.ok()) {
    std::cout << result1 << "\n";
  }

  ctx.clear_exception();
  ctx.root()["world"] = 10;
  ctx.root()["x"] = ctx.obj();
  ctx.root()["x"].get().to_object()[12] = 30;
  const auto result2 = ctx.eval_script("1 + 2 + world + x[12]").to_number();
  if (ctx.ok()) {
    std::cout << result2 << "\n";
  }

  ctx.clear_exception();
  const auto result3 =
      ctx.eval_script("x => x**2").to_object().call(10).to_number();
  if (ctx.ok()) {
    std::cout << result3 << "\n";
  }

  ctx.clear_exception();
  ctx.root()["run"] =
      ctx.callable([](auto ctx, auto, auto args, auto exception) {
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
    std::cout << result4 << "\n";
  }

  ctx.clear_exception();
  ctx.eval_script("run(15, 16)");
  const auto result5 =
      ctx.get_exception().to_object()["stack"].get().to_string();
  std::cout << result5 << "\n";

  ctx.clear_exception();
  const auto result61 = ctx.eval_script("run.constructor.name").to_string();
  if (ctx.ok()) {
    std::cout << result61 << "\n";
  }

  ctx.clear_exception();
  ctx.root()["cont"] = ctx.container<int>(7);
  const auto result62 = ctx.eval_script("cont.constructor.name").to_string();
  if (ctx.ok()) {
    std::cout << result62 << "\n";
  }

  std::cout << ctx.root()["run"].get().to_object().is_function() << " "
            << ctx.root()["cont"].get().to_object().is_function() << "\n";

  auto str_container = ctx.container<std::string>("Hello");
  std::cout << str_container.is_container<std::string>() << " "
            << *str_container.get_contained<std::string>() << "\n";

  ctx.clear_exception();
  std::ifstream input{"../test/sudoku_v1.js"};
  std::ostringstream buffer;
  buffer << input.rdbuf();
  std::cout << buffer.str().length() << "\n";
  auto start = std::chrono::high_resolution_clock::now();
  ctx.eval_script(buffer.str(), "sudoku_v1.js");
  if (!ctx.ok()) {
    std::cout << "Timed execution: exception!\n";
  } else {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << "\n";
  }

  ctx.eval_script(u8"\"你好世界\"; throw new Error();");
  const auto result7 =
      ctx.get_exception().to_object()["stack"].get().to_string();
  std::cout << result7 << "\n";

  return 0;
}

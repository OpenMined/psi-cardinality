#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"

namespace private_set_intersection {
namespace {

void BM_ServerSetup(benchmark::State& state, double fpr,
                    bool reveal_intersection) {
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  int num_client_inputs = 10000;
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::ServerSetup setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    setup =
        server->CreateSetupMessage(fpr, num_client_inputs, inputs).ValueOrDie();
    ::benchmark::DoNotOptimize(setup);
    elements_processed += num_inputs;
  }
  state.counters["SetupSize"] = benchmark::Counter(
      static_cast<double>(setup.ByteSizeLong()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs, and the captured argument is the false
// positive rate for 10k client queries.
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 size, 0.000001, false)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 intersection, 0.000001, true)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);

void BM_ClientCreateRequest(benchmark::State& state, bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::Request request;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    request = client->CreateRequest(inputs).ValueOrDie();
    ::benchmark::DoNotOptimize(request);
    elements_processed += num_inputs;
  }
  state.counters["RequestSize"] = benchmark::Counter(
      static_cast<double>(request.ByteSizeLong()),
      benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientCreateRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);
BENCHMARK_CAPTURE(BM_ClientCreateRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);

void BM_ServerProcessRequest(benchmark::State& state,
                             bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::Request request = client->CreateRequest(inputs).ValueOrDie();
  psi_proto::Response response;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    response = server->ProcessRequest(request).ValueOrDie();
    ::benchmark::DoNotOptimize(response);
    elements_processed += num_inputs;
  }
  state.counters["ResponseSize"] = benchmark::Counter(
      static_cast<double>(response.ByteSizeLong()),
      benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ServerProcessRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);
BENCHMARK_CAPTURE(BM_ServerProcessRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);

void BM_ClientProcessResponse(benchmark::State& state,
                              bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  double fpr = 1. / (1000000);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::ServerSetup setup =
      server->CreateSetupMessage(fpr, num_inputs, inputs).ValueOrDie();
  psi_proto::Request request = client->CreateRequest(inputs).ValueOrDie();
  psi_proto::Response response = server->ProcessRequest(request).ValueOrDie();
  int64_t elements_processed = 0;
  for (auto _ : state) {
    if (reveal_intersection) {
      auto intersection = client->GetIntersection(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(intersection);
    } else {
      int64_t count = client->GetIntersectionSize(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(count);
    }
    elements_processed += num_inputs;
  }
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size, false)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection, true)
    ->RangeMultiplier(10)
    ->Range(100000, 1000000);

void BM_ClientProcessResponseAsymmetric(benchmark::State& state,
                                        bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int server_num_inputs = 1000000;
  int client_num_inputs = 1000;
  double fpr = 1. / (1000000);
  std::vector<std::string> server_inputs(server_num_inputs);
  std::vector<std::string> client_inputs(client_num_inputs);
  for (int i = 0; i < server_num_inputs; i++) {
    server_inputs[i] = absl::StrCat("Element", i);
  }
  for (int i = 0; i < client_num_inputs; i++) {
    client_inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::ServerSetup setup =
      server->CreateSetupMessage(fpr, server_num_inputs, server_inputs)
          .ValueOrDie();
  psi_proto::Request request =
      client->CreateRequest(client_inputs).ValueOrDie();
  psi_proto::Response response = server->ProcessRequest(request).ValueOrDie();
  int64_t elements_processed = 0;
  for (auto _ : state) {
    if (reveal_intersection) {
      auto intersection = client->GetIntersection(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(intersection);
    } else {
      int64_t count = client->GetIntersectionSize(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(count);
    }
    elements_processed += client_num_inputs;
  }
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientProcessResponseAsymmetric, size, false)
    ->RangeMultiplier(10)
    ->Range(1000000, 1000000);
BENCHMARK_CAPTURE(BM_ClientProcessResponseAsymmetric, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1000000, 1000000);

}  // namespace
}  // namespace private_set_intersection

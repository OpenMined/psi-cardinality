#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "private_set_intersection/c/psi_client.h"
#include "private_set_intersection/c/psi_server.h"

namespace private_set_intersection {
namespace {

int num_server_inputs = 1000000;
double FPR = 0.000000001;

void BM_ServerSetup(benchmark::State &state, double fpr,
                    bool reveal_intersection) {
  psi_server_ctx server_;
  char *err;
  psi_server_create_with_new_key(reveal_intersection, &server_, &err);

  int num_client_inputs = state.range(0);
  std::vector<std::string> orig_inputs(num_server_inputs);
  std::vector<psi_server_buffer_t> inputs(num_server_inputs);

  for (int i = 0; i < num_server_inputs; i++) {
    orig_inputs[i] = absl::StrCat("Element", i);
    inputs[i] = {orig_inputs[i].c_str(), orig_inputs[i].size()};
  }
  std::string setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *server_setup = nullptr;
    size_t server_setup_buff_len = 0;
    psi_server_create_setup_message(server_, fpr, num_client_inputs,
                                    inputs.data(), inputs.size(), &server_setup,
                                    &server_setup_buff_len, &err);

    ::benchmark::DoNotOptimize(server_setup);
    ::benchmark::ClobberMemory();
    elements_processed += num_server_inputs;
    setup = server_setup;

    free(server_setup);
  }
  state.counters["SetupSize"] = benchmark::Counter(
      static_cast<double>(setup.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}

// Range is for the number of inputs, and the captured argument is the false
// positive rate for 10k client queries.
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 intersection, FPR, true)
    ->RangeMultiplier(10)
    ->Range(1000, 100000);

void BM_ClientCreateRequest(benchmark::State &state, bool reveal_intersection) {
  psi_client_ctx client_;
  char *err;
  psi_client_create_with_new_key(reveal_intersection, &client_, &err);

  int num_inputs = state.range(0);
  std::vector<std::string> inputs_orig(num_inputs);
  std::vector<psi_client_buffer_t> inputs(num_inputs);

  for (int i = 0; i < num_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }
  std::string request;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *client_request = {0};
    size_t req_len = 0;
    char *err;
    psi_client_create_request(client_, inputs.data(), inputs.size(),
                              &client_request, &req_len, &err);

    ::benchmark::DoNotOptimize(client_request);
    ::benchmark::ClobberMemory();
    elements_processed += num_inputs;
    request = client_request;
    free(client_request);
  }
  state.counters["RequestSize"] = benchmark::Counter(
      static_cast<double>(request.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientCreateRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1000, 100000);

void BM_ServerProcessRequest(benchmark::State &state,
                             bool reveal_intersection) {
  psi_client_ctx client_;
  psi_server_ctx server_;
  char *err;

  psi_client_create_with_new_key(reveal_intersection, &client_, &err);
  psi_server_create_with_new_key(reveal_intersection, &server_, &err);

  int num_inputs = state.range(0);
  std::vector<std::string> inputs_orig(num_inputs);
  std::vector<psi_client_buffer_t> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }

  char *client_request = nullptr;
  size_t req_len = 0;
  psi_client_create_request(client_, inputs.data(), inputs.size(),
                            &client_request, &req_len, &err);

  std::string response;

  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *server_response = nullptr;
    size_t response_len = 0;
    psi_server_process_request(server_, {client_request, req_len},
                               &server_response, &response_len, &err);
    ::benchmark::DoNotOptimize(server_response);
    ::benchmark::ClobberMemory();

    elements_processed += num_inputs;
    response = server_response;
    free(server_response);
  }
  state.counters["ResponseSize"] = benchmark::Counter(
      static_cast<double>(response.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);

  free(client_request);
}

// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ServerProcessRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1000, 100000);

void BM_ClientProcessResponse(benchmark::State &state,
                              bool reveal_intersection) {
  psi_client_ctx client_;
  psi_server_ctx server_;
  char *err;
  psi_client_create_with_new_key(reveal_intersection, &client_, &err);
  psi_server_create_with_new_key(reveal_intersection, &server_, &err);

  int num_client_inputs = state.range(0);
  double fpr = FPR;
  std::vector<std::string> inputs_orig(num_server_inputs);
  std::vector<psi_server_buffer_t> srv_inputs(num_server_inputs);
  std::vector<psi_client_buffer_t> cl_inputs(num_client_inputs);
  for (int i = 0; i < num_server_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    srv_inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }
  for (int i = 0; i < num_client_inputs; i++) {
    cl_inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }

  char *server_setup = nullptr;
  size_t server_setup_buff_len = 0;
  psi_server_create_setup_message(server_, fpr, num_client_inputs,
                                  srv_inputs.data(), srv_inputs.size(),
                                  &server_setup, &server_setup_buff_len, &err);

  char *client_request = nullptr;
  size_t req_len = 0;
  psi_client_create_request(client_, cl_inputs.data(), cl_inputs.size(),
                            &client_request, &req_len, &err);

  char *server_response = nullptr;
  size_t response_len = 0;
  psi_server_process_request(server_, {client_request, req_len},
                             &server_response, &response_len, &err);

  int64_t elements_processed = 0;
  for (auto _ : state) {
    if (reveal_intersection) {
      int64_t *out;
      size_t count = 0;
      psi_client_get_intersection(
          client_, {server_setup, server_setup_buff_len},
          {server_response, response_len}, &out, &count, &err);
      ::benchmark::DoNotOptimize(count);
    } else {
      int64_t count = 0;
      psi_client_get_intersection_size(
          client_, {server_setup, server_setup_buff_len},
          {server_response, response_len}, &count, &err);
      ::benchmark::DoNotOptimize(count);
    }
    elements_processed += num_client_inputs;
  }

  free(server_setup);
  free(server_response);
  free(client_request);

  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1000, 100000);

}  // namespace
}  // namespace private_set_intersection

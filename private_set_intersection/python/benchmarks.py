import pytest
import sys
import private_set_intersection.python as psi

server_cnt = 1000000
FPR = 0.000000001


def helper_client_create_request(cnt, reveal_intersection, c, inputs):
    req = c.CreateRequest(inputs)


@pytest.mark.parametrize("cnt", [10 ** 3, 10 ** 4, 10 ** 5])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_client_create_request(cnt, reveal_intersection, benchmark):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    inputs = ["Element " + str(i) for i in range(cnt)]
    benchmark(helper_client_create_request, cnt, reveal_intersection, c, inputs)


def helper_client_process_response(cnt, reveal_intersection, setup, resp, c):
    if reveal_intersection:
        intersection = c.GetIntersection(setup, resp)
    else:
        intersection = c.GetIntersectionSize(setup, resp)


@pytest.mark.parametrize("cnt", [10 ** 3, 10 ** 4, 10 ** 5])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_client_process_response(cnt, reveal_intersection, benchmark):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = FPR
    inputs = ["Element " + str(i) for i in range(cnt)]
    server_inputs = ["Element " + str(i) for i in range(server_cnt)]
    req = c.CreateRequest(inputs)

    setup = s.CreateSetupMessage(fpr, len(inputs), server_inputs)
    request = c.CreateRequest(inputs)
    resp = s.ProcessRequest(request)

    benchmark(helper_client_process_response, cnt, reveal_intersection, setup, resp, c)


def helper_server_setup(client_cnt, fpr, reveal_intersection, s, items):
    setup = s.CreateSetupMessage(fpr, client_cnt, items)


@pytest.mark.parametrize("client_cnt", [1000, 10000, 100000])
@pytest.mark.parametrize("fpr", [FPR])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_server_setup(client_cnt, fpr, reveal_intersection, benchmark):
    s = psi.server.CreateWithNewKey(reveal_intersection)
    items = ["Element " + str(2 * i) for i in range(server_cnt)]
    benchmark(helper_server_setup, client_cnt, fpr, reveal_intersection, s, items)


def helper_server_process_request(cnt, reveal_intersection, request, s):
    resp = s.ProcessRequest(request)


@pytest.mark.parametrize("client_cnt", [1000, 10000, 100000])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_server_process_request(client_cnt, reveal_intersection, benchmark):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = FPR
    client_inputs = ["Element " + str(i) for i in range(client_cnt)]
    server_inputs = ["Element " + str(i) for i in range(server_cnt)]
    req = c.CreateRequest(client_inputs)

    setup = s.CreateSetupMessage(fpr, len(client_inputs), server_inputs)
    request = c.CreateRequest(client_inputs)
    benchmark(helper_server_process_request, client_cnt, reveal_intersection, request, s)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))

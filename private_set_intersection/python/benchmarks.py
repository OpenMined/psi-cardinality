import pytest
import sys
import private_set_intersection.python as psi


def helper_client_create_request(cnt, reveal_intersection, c, inputs):
    req = c.CreateRequest(inputs)


@pytest.mark.parametrize("cnt", [100000])
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


@pytest.mark.parametrize("cnt", [100000])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_client_process_response(cnt, reveal_intersection, benchmark):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]
    req = c.CreateRequest(inputs)

    setup = s.CreateSetupMessage(fpr, len(inputs), inputs)
    request = c.CreateRequest(inputs)
    resp = s.ProcessRequest(request)

    benchmark(helper_client_process_response, cnt, reveal_intersection, setup, resp, c)


def helper_server_setup(cnt, fpr, reveal_intersection, s, items):
    setup = s.CreateSetupMessage(fpr, 10000, items)


@pytest.mark.parametrize("cnt", [100000])
@pytest.mark.parametrize("fpr", [0.000001])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_server_setup(cnt, fpr, reveal_intersection, benchmark):
    s = psi.server.CreateWithNewKey(reveal_intersection)
    items = ["Element " + str(2 * i) for i in range(cnt)]
    benchmark(helper_server_setup, cnt, fpr, reveal_intersection, s, items)


def helper_server_process_request(cnt, reveal_intersection, request, s):
    resp = s.ProcessRequest(request)


@pytest.mark.parametrize("cnt", [100000])
@pytest.mark.parametrize("reveal_intersection", [True])
def test_server_process_request(cnt, reveal_intersection, benchmark):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]
    req = c.CreateRequest(inputs)

    setup = s.CreateSetupMessage(fpr, len(inputs), inputs)
    request = c.CreateRequest(inputs)
    benchmark(helper_server_process_request, cnt, reveal_intersection, request, s)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))

package client

import (
	"bytes"
	"github.com/openmined/psi/pb"
	"github.com/openmined/psi/server"
	"regexp"
	"testing"
)

func generateSet(in []int64) map[int]struct{} {
	out := map[int]struct{}{}
	for _, item := range in {
		out[int(item)] = struct{}{}
	}

	return out
}
func testClientSanity(t *testing.T, revealIntersection bool) {
	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	key, err := client.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a PSI client key %v", err)
	}

	newClient, err := CreateFromKey(key, revealIntersection)
	if err != nil || newClient == nil {
		t.Errorf("Failed to create a PSI client from key %v", err)
	}

	newKey, err := newClient.GetPrivateKeyBytes()
	if err != nil {
		t.Errorf("Failed to create a new PSI client key %v", err)
	}
	if !bytes.Equal(key, newKey) {
		t.Errorf("new client invalid")
	}
	client.Destroy()
	for i := 0; i < 5; i++ {
		client.Destroy()
	}

	matched, _ := regexp.MatchString(`[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?`, client.Version())
	if !matched {
		t.Errorf("Got invalid version %v", client.Version())
	}
}

func TestClientSanity(t *testing.T) {
	testClientSanity(t, true)
	testClientSanity(t, false)
}

func testClientFailure(t *testing.T, revealIntersection bool) {
	c := &PsiClient{}
	_, err := c.CreateRequest([]string{"dummy"})
	if err == nil {
		t.Errorf("CreateRequest with an invalid context should fail")
	}
	dummy1 := &psi_proto.ServerSetup{}
	dummy2 := &psi_proto.Response{}
	if revealIntersection {
		_, err = c.GetIntersection(dummy1, dummy2)
		if err == nil {
			t.Errorf("GetIntersection with an invalid context should fail")
		}
	} else {
		_, err = c.GetIntersectionSize(dummy1, dummy2)
		if err == nil {
			t.Errorf("GetIntersectionSize with an invalid context should fail")
		}
	}
}

func TestClientFailure(t *testing.T) {
	testClientFailure(t, false)
	testClientFailure(t, true)
}

func testClientServer(t *testing.T, revealIntersection bool) {
	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		t.Errorf("Failed to create a PSI client %v", err)
	}

	server, err := server.CreateWithNewKey(revealIntersection)
	if err != nil || server == nil {
		t.Errorf("Failed to create a PSI server %v", err)
	}

	generateItems := func(cnt int, m int) (int, []string) {
		items := []string{}
		for i := 0; i < cnt; i++ {
			items = append(items, "Element "+string(m*i))
		}
		return cnt, items
	}
	cntClientItems, clientItems := generateItems(1000, 1)
	_, serverItems := generateItems(10000, 2)

	fpr := 1. / (1000000000)
	setup, err := server.CreateSetupMessage(fpr, int64(cntClientItems), serverItems)
	if err != nil {
		t.Errorf("failed to create setup msg %v", err)
	}
	request, err := client.CreateRequest(clientItems)
	if err != nil {
		t.Errorf("failed to create request %v", err)
	}
	serverResp, err := server.ProcessRequest(request)
	if err != nil {
		t.Errorf("failed to process request %v", err)
	}
	if revealIntersection {
		intersection, err := client.GetIntersection(setup, serverResp)
		if err != nil {
			t.Errorf("failed to compute intersection %v", err)
		}
		intersectionSet := generateSet(intersection)
		for idx := 0; idx < cntClientItems; idx++ {
			_, ok := intersectionSet[idx]
			if ok != (idx%2 == 0) {
				t.Errorf("Invalid intersection for item %v %v", idx, ok)
			}
		}
	} else {
		intersectionCnt, err := client.GetIntersectionSize(setup, serverResp)
		if err != nil {
			t.Errorf("failed to compute intersection size %v", err)
		}

		if int(intersectionCnt) < (cntClientItems / 2) {
			t.Errorf("Invalid intersection. expected lower bound %v. got %v", (cntClientItems / 2), intersectionCnt)
		}

		if float64(intersectionCnt) > float64(cntClientItems/2)*float64(1.1) {
			t.Errorf("Invalid intersection. expected upper bound %v. got %v", float64(cntClientItems/2)*float64(1.1), intersectionCnt)
		}

	}
}

func TestClientServer(t *testing.T) {

	testClientServer(t, false)
	testClientServer(t, true)
}

var result *psi_proto.Request

func benchmarkClientCreateRequest(cnt int, revealIntersection bool, b *testing.B) {
	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		b.Errorf("failed to get client")
	}
	inputs := []string{}
	for i := 0; i < cnt; i++ {
		inputs = append(inputs, "Element "+string(i))
	}

	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		request, err := client.CreateRequest(inputs)
		if err != nil {
			b.Errorf("failed to generate request")
		}

		//ugly hack for preventing compiler optimizations
		result = request

		total += cnt
		b.ReportMetric(float64(request.XXX_Size()), "RequestSize")

	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientCreateRequest1000(b *testing.B) { benchmarkClientCreateRequest(1000, true, b) }
func BenchmarkClientCreateRequest10000(b *testing.B) { benchmarkClientCreateRequest(10000, true, b) }
func BenchmarkClientCreateRequest100000(b *testing.B) { benchmarkClientCreateRequest(100000, true, b) }

var dummyInt64 int64

func benchmarkClientProcessResponse(clientCnt int, revealIntersection bool, b *testing.B) {
	client, err := CreateWithNewKey(revealIntersection)
	if err != nil || client == nil {
		b.Errorf("failed to get client")
	}
	server, err := server.CreateWithNewKey(revealIntersection)
	if err != nil || server == nil {
		b.Errorf("failed to get server")
	}

	fpr := 1. / (1000000000)

    serverCnt := 1000000
	clientInputs := []string{}
	serverInputs := []string{}
	for i := 0; i < serverCnt; i++ {
		serverInputs = append(serverInputs, "Element "+string(i))
	}
	for i := 0; i < clientCnt; i++ {
		clientInputs = append(clientInputs, "Element "+string(i))
	}

	setup, err := server.CreateSetupMessage(fpr, int64(clientCnt), serverInputs)
	if err != nil {
		b.Errorf("failed to create setup msg %v", err)
	}
	request, err := client.CreateRequest(clientInputs)
	if err != nil {
		b.Errorf("failed to create request %v", err)
	}
	serverResp, err := server.ProcessRequest(request)
	if err != nil {
		b.Errorf("failed to process request %v", err)
	}

	b.ReportAllocs()
	total := 0
	for n := 0; n < b.N; n++ {
		if revealIntersection {
			intersection, err := client.GetIntersection(setup, serverResp)
			if err != nil {
				b.Errorf("failed to compute intersection %v", err)
			}
			intersectionSet := generateSet(intersection)
			for idx := 0; idx < clientCnt; idx++ {
				_, ok := intersectionSet[idx]
				if !ok {
					b.Errorf("Invalid intersection for item %v", idx)
				}
			}
		} else {
			intersectionCnt, err := client.GetIntersectionSize(setup, serverResp)
			if err != nil {
				b.Errorf("failed to process response %v", err)
			}
			total += clientCnt
			//ugly hack for preventing compiler optimizations
			dummyInt64 = intersectionCnt
		}
	}
	b.ReportMetric(float64(total), "ElementsProcessed")
}

func BenchmarkClientProcessResponseIntersection1000(b *testing.B) {
	benchmarkClientProcessResponse(1000, true, b)
}

func BenchmarkClientProcessResponseIntersection10000(b *testing.B) {
	benchmarkClientProcessResponse(10000, true, b)
}

func BenchmarkClientProcessResponseIntersection100000(b *testing.B) {
	benchmarkClientProcessResponse(100000, true, b)
}

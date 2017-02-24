#include <array>
#include <bitset>
#include <random>
template<int N>
std::array<std::bitset<N>, N> hard_sub(std::array<int, N> deg) {
	int M=0;
	for (auto itr : deg) {
		M += itr;
	}
	M /= 2;

	std::random_device rnddev;
	std::mt19937_64 mtrnd;
	int seeds[624];
	for (auto& itr : seeds) {
		itr = rnddev();
	}
	std::seed_seq seed(&seeds[0], &seeds[624]);
	mtrnd.seed(seed);

	bool is_failed;
	std::array<int,N> remDeg;
	auto nnz = [](std::array<int, N> deg)->int {
		int cnt=0;
		for (auto itr : deg) {
			if (itr != 0) {
				cnt++;
			}
		}
		return cnt;
	};
	auto nnzm = [](std::array<std::bitset<N>, N> graph, std::bitset<N> remNodes)-> int {
		int cnt=0;
		for (int i = 0; i < N;i++) {
			if (remNodes[i]) {
				cnt += (graph[i] & remNodes).count();
			}
		}
		return cnt;
	};
	int remN;
	std::bitset<N> remNodes;
	std::array<std::bitset<N>, N> randGraph;
	
	do {
		is_failed = false;
		int remM = M;		
		remDeg = deg;
		remN = nnz(remDeg);
		remNodes.set();
		for (auto& itr : randGraph) {
			itr.reset();
		}

		while (remN > 1) {
			int rid = std::discrete_distribution<>(remDeg.begin(), remDeg.end())(mtrnd);
			int tmp = remDeg[rid]; remDeg[rid] = 0;
			int cid = std::discrete_distribution<>(remDeg.begin(), remDeg.end())(mtrnd);
			remDeg[rid] = tmp;
			if (randGraph[rid][cid]) {
				continue;
			}

			remDeg[rid]--; remDeg[cid]--;
			if (remDeg[rid] == 0) {
				remNodes.reset(rid);
				remN--;
			}
			if (remDeg[cid] == 0) {
				remNodes.reset(cid);
				remN--;
			}

			randGraph[rid].set(cid);
			randGraph[cid].set(rid);
			remM--;
			if (remN > 0 & !((remN*remN - remN) - nnzm(randGraph, remNodes) >= 2 * remM)) {
				is_failed = true;
				break;
			}
		}
	} while (is_failed);
	return randGraph;
}


#include "main.ih"

#define DEDEKINDNUMBER 7
#define BUFFSIZE 32


static double timer(void)
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return tm.tv_sec + tm.tv_usec / 1000000.0;
}

template <typename T, typename Alloc>
std::ostream &operator<<(std::ostream &out, std::set<T, Alloc> const &rhs)
{
	out << '{';
	for (typename std::set<T>::iterator iter = rhs.begin();
			iter != rhs.end();
				++iter)
	{
		out << ((iter != rhs.begin()) ? ", " : "") << *iter;
	}
	out << '}';
	return out;
}

template <typename T, size_t size>
std::ostream &operator<<(std::ostream &out, std::array<T, size> const &rhs)
{
	out << '{';
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter)
	{
		out << ((iter != rhs.begin()) ? ", " : "") << *iter;
	}
	out << '}';
	return out;
}

bool vectorLess(set<size_t> const &lhs, set<size_t> const &rhs)
{
	if (lhs.size() == rhs.size())
	{
		for (auto iter = lhs.begin(), iter2 = rhs.begin(); iter != lhs.end();
				++iter, ++iter2)
		{
			if (*iter != *iter2)
			{
				return *iter < *iter2;
			}
		}
	}

	return lhs.size() < rhs.size();
}

void powersetRec(std::vector<std::set<size_t>> &result,
					  std::set<size_t> current, size_t const n)
{
	if (n == 0)
	{
		result.push_back(current);
		return;
	}

	size_t newN(n - 1);
	powersetRec(result, current, newN);
	current.insert(newN);
	powersetRec(result, current, newN);
}

template <size_t size>
std::array<std::set<size_t>, size> powerset(size_t const n)
{
	std::vector<std::set<size_t>> resultVector;
	std::set<size_t> current;
	powersetRec(resultVector, current, n);
	sort(resultVector.begin(), resultVector.end(), vectorLess);

	std::array<std::set<size_t>, size> result;
	copy(resultVector.begin(), resultVector.end(), result.begin());

	return result;
}

template <size_t size>
std::set<size_t> permute(std::array<size_t, size> permutation, std::set<size_t> elem)
{
	std::set<size_t> result;
	for (auto iter = elem.begin(); iter != elem.end(); ++iter)
	{
		result.insert(permutation[*iter]);
	}
	return result;
}

template <size_t size>
std::bitset<size> permute(std::array<size_t, size> permutation,
		std::bitset<size> elem)
{
	std::bitset<size> result;
	for (size_t idx = 0; idx != result.size(); ++idx)
	{
		result[idx] = elem[permutation[idx]];
	}
	return result;
}

template <size_t size, size_t size2>
std::array<size_t, size2> mbfPermutation(std::array<size_t, size> permutation,
		std::array<std::set<size_t>, size2> ps)
{
	std::array<size_t, size2> result;
	size_t idx = 0;
	for (auto iter = ps.begin(); iter != ps.end(); ++iter)
	{
		std::set<size_t> tmp = permute(permutation, *iter);
		result[idx++] = find(ps.begin(), ps.end(), tmp) - ps.begin();
	}
	return result;
}

template <size_t size>
set<bitset<size>, BitSetLess> permutations(bitset<size> bs,
		vector<array<size_t, size>> perms)
{
	set<bitset<size>, BitSetLess> result;
	for (auto iter = perms.begin(); iter!= perms.end(); ++iter)
	{
		bitset<size> temp = permute(*iter, bs);
		result.insert(temp);
	}
	return result;
}

int main(int argc, char **argv)
{
	size_t const n = 4;
	size_t const p = pow(2, n);
	array<size_t, n> permutation;
	vector<array<size_t, n>> perms;

	for (size_t idx = 0; idx != n; ++idx)
	{
		permutation[idx] = idx;
	}

	std::array<std::set<size_t>, p> powerSet = powerset<p>(n);

	cout << powerSet << '\n';
	vector<array<size_t, p>> mbfPermutations;
	do
	{
		perms.push_back(permutation);
		mbfPermutations.push_back(mbfPermutation(permutation, powerSet));
	}
	while (std::next_permutation(permutation.begin(), permutation.end()));

	std::vector<std::bitset<1>> D0({std::bitset<1>(0),
				std::bitset<1>(1)});
	auto D1 = Dedekind::generate(D0);
	auto D2 = Dedekind::generate(D1);
	auto D3 = Dedekind::generate(D2);
	auto D4 = Dedekind::generate(D3);
	auto D5 = Dedekind::generate(D4);
	auto D6 = Dedekind::generate(D5);

	cout << D6.size() << '\n';

	vector<set<bitset<p>, BitSetLess>> rn;
	for (auto iter = D4.begin(); iter != D4.end(); ++iter)
	{
		auto permuted = permutations(*iter, mbfPermutations);
		if (find(rn.begin(), rn.end(), permuted) == rn.end())
		{
			rn.push_back(permuted);
		}
	}

	cout << rn << '\n';

	MPI::Init(argc, argv);
	MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);

	size_t rank = 0;
	size_t size = 1;
	try
	{
		rank = MPI::COMM_WORLD.Get_rank();
		size = MPI::COMM_WORLD.Get_size();
		// cout << "I am " << rank << " of " << size << '\n';
	}
	catch (MPI::Exception e)
	{
		cerr << "MPI ERROR: " << e.Get_error_code()
			  << " - " << e.Get_error_string() << endl;
	}


	if (argc == 2 && argv[1][0] == 'b')
	{
		double timer1 = timer();

		mpz_class result = Dedekind::monotoneSubsets<DEDEKINDNUMBER>(rank, size);

		if (rank == 0)
		{
			size_t toReceive = size - 1;
			char buffer[BUFFSIZE];
			while (toReceive--)
			{
				MPI::Status status;
				MPI::COMM_WORLD.Recv(buffer, BUFFSIZE, MPI::CHAR, MPI::ANY_SOURCE,
						Dedekind::BIGINTTAG, status);

				mpz_class tmp(buffer);
				result += tmp;
			}
		}
		else
		{
			MPI::COMM_WORLD.Isend(result.get_str().c_str(), BUFFSIZE, MPI::CHAR, 0,
				Dedekind::BIGINTTAG);
		}

		double timer2 = timer();
		cout << "Rank: " << rank << ": " << result << " in "
			  << timer2 - timer1 << "s\n";

		// cout << Dedekind::monotoneSubsets<DEDEKINDNUMBER>(rank, size) << '\n';
	}
	else
	{
		if (rank == 0)
		{
			size_t n = 0;
			if (argc == 2)
			{
				stringstream ss(argv[1]);
				ss >> n;
			}
			cout << Dedekind::monotoneSubsets(n);
		}
	}
	MPI::Finalize();
}

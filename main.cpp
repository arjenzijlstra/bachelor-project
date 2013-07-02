
#include "main.ih"

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

template <size_t size>
struct PowerSet
{
	static void powersetRec(vector<bitset<size>> &bset);
};

template <size_t size>
void PowerSet<size>::powersetRec(vector<bitset<size>> &bset)
{
	vector<bitset<size - 1>> current;
	PowerSet<size - 1>::powersetRec(current);

	for (auto iter = current.begin(); iter != current.end(); ++iter)
	{
		bitset<size> tmp((*iter).to_ulong() + (1 << (size - 1)));
		bset.push_back(tmp);
	}
	for (auto iter = current.begin(); iter != current.end(); ++iter)
	{
		bitset<size> tmp((*iter).to_ulong());
		bset.push_back(tmp);
	}
}

template <>
struct PowerSet<0>
{
	static void powersetRec(vector<bitset<0>> &bset);
};

void PowerSet<0>::powersetRec(vector<bitset<0>> &bset)
{
	bset.push_back(bitset<0>());
}

template <size_t size>
vector<bitset<size>> powerset()
{
	vector<bitset<size>> result;
	PowerSet<size>::powersetRec(result);
	return result;
}


int main(int argc, char **argv)
{
	MPI::Init(argc, argv);
	MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);

	size_t rank = 0;
	size_t size = 1;
	try
	{
		rank = MPI::COMM_WORLD.Get_rank();
		size = MPI::COMM_WORLD.Get_size();
	}
	catch (MPI::Exception const &exception)
	{
		cerr << "MPI error: " << exception.Get_error_code() << " - "
				<< exception.Get_error_string() << endl;
	}


	cout << powerset<2>() << '\n';

	if (argc == 3 && string(argv[1]) == "-d")
	{
		size_t n = 2;
		stringstream ss(argv[2]);
		ss >> n;

		double start = MPI::Wtime();

		Dedekind::UInt128 result;
		switch (n)
		{
		// 	// case 3:
		// 	// 	result = Dedekind::monotoneSubsets<3>(rank, size);
		// 	// 	break;
		// 	// case 4:
		// 	// 	result = Dedekind::monotoneSubsets<4>(rank, size);
		// 	// 	break;
		// 	// case 5:
		// 	// 	result = Dedekind::monotoneSubsets<5>(rank, size);
		// 	// 	break;
			case 6:
				result = Dedekind::monotoneSubsets<6>(rank, size);
				break;
			case 7:
				result = Dedekind::monotoneSubsets<7>(rank, size);
				break;
			// case 8:
			// 	result = Dedekind::monotoneSubsets<8>(rank, size);
		}


		double end = MPI::Wtime();
		cerr << "Rank " << rank << " done! Result: " << result << " in "
			  << end - start << "s\n";

		// reduce over all processes
		if (rank == 0)
		{
			size_t toReceive = size;
			while (--toReceive)
			{
				// send the high and the low part of the result
				uint_fast64_t lohi[2];

				MPI::Status status;
				MPI::COMM_WORLD.Recv(lohi, 2, MPI::UNSIGNED_LONG,
						MPI::ANY_SOURCE, Dedekind::BIGINTTAG, status);

				Dedekind::UInt128 tmp(lohi[0], lohi[1]);
				result += tmp;
			}

			double final = MPI::Wtime();
			cout << "d" << n << " = " << result
				<< " (in " << final - start << "s)\n";
		}
		else
		{
			uint_fast64_t lohi[2];
			lohi[0] = result.lo();
			lohi[1] = result.hi();
			MPI::COMM_WORLD.Send(&lohi, 2, MPI::UNSIGNED_LONG, 0,
					Dedekind::BIGINTTAG);


			double final = MPI::Wtime();
			cerr << "Rank " << rank << " exiting! Total: "
					<< final - start << "s\n";
		}
	}
	else
	{
		cout << "Usage: ./project -d x \n"
				<< "Where x in [2..n) is the Dedekind Number to calculate.\n";
	}
	MPI::Finalize();
}

// typedef Dedekind::UInt128 (*fptr)(int, int); <-- pas aan zodat klopt

// template<Int a = 3>
// fptr findFunction(int b)
// {
//   if(a == b)
//     return Dedekind::monotoneSubsets<a>;
//   else
//     return findFunction<a + 1>( b );
// }


#include "main.ih"

#define DEDEKINDNUMBER 7


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
	catch (MPI::Exception e)
	{
		cerr << "MPI ERROR: " << e.Get_error_code()
			  << " - " << e.Get_error_string() << endl;
	}


	if (argc == 3 && string(argv[1]) == "-b")
	{
		size_t n = 2;
		stringstream ss(argv[2]);
		ss >> n;

		double timer1 = timer();

		//Dedekind::UInt128 result;
		mpz_class result = 0;
		switch (n)
		{
			// case 3:
			// 	result = Dedekind::monotoneSubsets<3>(rank, size);
			// 	break;
			// case 4:
			// 	result = Dedekind::monotoneSubsets<4>(rank, size);
			// 	break;
			// case 5:
			// 	result = Dedekind::monotoneSubsets<5>(rank, size);
			// 	break;
			case 6:
				result = Dedekind::monotoneSubsets<6>(rank, size);
				break;
			case 7:
				result = Dedekind::monotoneSubsets<7>(rank, size);
				break;
			case 8:
				result = Dedekind::monotoneSubsets<8>(rank, size);
		}


		if (rank == 0)
		{
			// size_t toReceive = size - 1;
			// while (toReceive--)
			// {
			// 	uint_fast64_t lohi[2];

			// 	MPI::Status status;
			// 	MPI::COMM_WORLD.Recv(lohi, 2, MPI::UNSIGNED_LONG,
			// 			MPI::ANY_SOURCE, Dedekind::BIGINTTAG, status);

			// 	Dedekind::UInt128 tmp(lohi[0], lohi[1]);
			// 	result += tmp;
			// }
		}
		else
		{
			// uint_fast64_t lohi[2];
			// lohi[0] = result.lo();
			// lohi[1] = result.hi();
			// MPI::COMM_WORLD.Send(&lohi, 2, MPI::UNSIGNED_LONG, 0,
			// 		Dedekind::BIGINTTAG);
		}

		double timer2 = timer();
		cout << "Rank: " << rank << ": " << result << " in "
			  << timer2 - timer1 << "s\n";
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

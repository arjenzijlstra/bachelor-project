
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

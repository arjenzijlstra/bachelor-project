
#include "main.ih"

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

	if (argc == 3 && string(argv[1]) == "-d")
	{
		size_t n = 2;
		stringstream ss(argv[2]);
		ss >> n;

		double start = MPI::Wtime();

		Dedekind::UInt128 result;// = findFunction(n)(rank, size);

		switch (n)
		{
			case 3:
				result = Dedekind::monotoneSubsets<3>(rank, size);
				break;
			case 4:
				result = Dedekind::monotoneSubsets<4>(rank, size);
				break;
			case 5:
				result = Dedekind::monotoneSubsets<5>(rank, size);
				break;
			case 6:
				result = Dedekind::monotoneSubsets<6>(rank, size);
				break;
			case 7:
				result = Dedekind::monotoneSubsets<7>(rank, size);
				break;
			case 8:
			 	result = Dedekind::monotoneSubsets<8>(rank, size);
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
			// send the high and the low part of the result
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
		cout << "Usage: mpirun -np N ./project -d X \n"
			<< "Where X in [2..n) is the Dedekind Number to calculate.\n"
			<< "And N is the number of processes you would like to use.\n\n"
			<< "Note: The program will also work when running normally "
			<< "(without using mpirun).\n"
			<< "In that case the program will just run on 1 core.\n";
	}
	MPI::Finalize();
}



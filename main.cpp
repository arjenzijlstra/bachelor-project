
#include "main.ih"


/*
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
*/

/*
std::set<size_t> powerset(std::set<size_t> base)
{

}

void powersetRec(set<size_t> &powerset, set<size_t> const &base, auto iter)
{
	if (base.)
}*/


int main(int argc, char **argv)
{

	// set<bitset<1>, BitSetLess> test({bitset<1>(1), bitset<1>(0)});
	// set<bitset<2>, BitSetLess> test2;

	// __gnu_parallel::for_each(test.begin(), test.end(),// std::inserter(test2, test2.begin()),
	//     [&](bitset<1> const &arg)
	//     {
	//         __gnu_parallel::for_each(test.begin(), test.end(),
	//         [&](bitset<1> const &arg2)
	//         {
	//             if (arg <= arg2)
	//             {
	//                 test2.insert(concatenate(arg, arg2));
	//             }
	//         });
	//     });
	// cerr << test2 << '\n';


	if (argc == 2 && argv[1][0] == 'b')
	{
		// size_t const sizes[8] = {1, 2, 4, 8, 16, 32, 64, 128};
		// set<bitset<1>, BitSetLess> D0({bitset<1>(0), bitset<1>(1)});
		// set<bitset<32>, BitSetLess> result;
		// for (size_t iter = 0; iter != n; ++iter)
		// {
		//     set<bitset<sizes[iter]>, BitSetLess> tmp;
		//     if (iter == 0)
		//     {
		//         tmp = generate(D0);
		//     }
		//     else
		//     {
		//         tmp = generateD(result);
		//     }
		//     result = tmp;
		// }

		// struct bitsetWrapper {
		//     bitset<1> *bit1;
		//     bitset<2> *bit2;

		//     set(int n) {
		//         switch (n) {
		//             case 1: return bit1;
		//         }
		//     }
		// };

		// set<bitset<1>, BitSetLess> result0({bitset<1>(0), bitset<1>(1)});
		// set<bitset<2>, BitSetLess> result1(generateD(result0));
		// set<bitset<4>, BitSetLess> result2(generateD(result1));
		// set<bitset<8>, BitSetLess> result3(generateD(result2));
		// set<bitset<16>, BitSetLess> result4(generateD(result3));
		// set<bitset<32>, BitSetLess> result5(generateD(result4));
		// set<bitset<64>, BitSetLess> result6(generateD(result5));
		// // set<bitset<128>, BitSetLess> result7(generateD(result6));

		// cout << result0 << '\n' << result0.size() << '\n';
		// cout << result1 << '\n' << result1.size() << '\n';
		// cout << result2 << '\n' << result2.size() << '\n';
		// cout << result3 << '\n' << result3.size() << '\n';
		// cout << result4.size() << '\n';
		// cout << result5.size() << '\n';
		// cout << result6.size() << '\n';

		DedekindBit dedekind;
		dedekind.generateMonotoneSubsets(0);

		//vector<bitset<1>> start({bitset<1>(0), bitset<1>(1)});
		//for (size_t idx = 0; idx != start.size(); ++idx)
		 //   cout << start[idx] << '\n';

	}
	else
	{
		size_t n = 0;
		if (argc == 2)
		{
			stringstream ss(argv[1]);
			ss >> n;
			// cout << n << '\n';
		}
		DedekindInt dedekind;
		dedekind.generateMonotoneSubsets(n);

		cout << dedekind.base().size() << '\n';
		cout << dedekind.base() << '\n';


		// set<set<set<size_t>, SetLess>, SetLess> result({{ }, {{ }}});

		// size_t k = 0;
		// // cout << "Computing Dedekind number: " << n << '\n';
		// for (size_t iter = 0; iter != n; ++iter)
		// {
		//     result = generate(result, k++);
		//     size_t dn = result.size();
		//     cout << "Number of elements: " << dn << '\n';
		//     if (dn < 100)
		//     {
		//         cout << result << '\n';
		//     }
		// }
		// cout << result.size() << '\n';
	}
}

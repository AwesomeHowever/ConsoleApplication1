// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cstdlib>
#include <windows.h>

typedef unsigned int ui; /* Основной тип данных для создания
матриц смежности есть беззнаковое целое. Обозначен ui
*/
class BM // класс матриц смежности
{
public:
	int *V;
	int Vsize;
	BM(ui** _A, int _vertex) { /*конструктор с вводом матрицы
	**_А-указатель на матрицу смежности;
	_vertex - число вершин графа
	*/
		int i, j;
		if (_vertex > 0 && _vertex < 1001)// число вершин не должно превышать 1000
		{
			vertex = _vertex; // инициализация числа вершин
			A = new ui * [vertex];// и массива для матрицы смежности

			V = new int[vertex];
			Vsize = vertex;//массив вершин

			for (i = 0; i < vertex; i++)
			{
				A[i] = new ui[vertex];
				V[i] = i + 1;
			}
		}


		for(i = 0; i < vertex; i++)
			for (j = 0; j < vertex; j++)
			{
				if (_A[i][j] != 0 && _A[i][j] != 1)// если элемент больше единицы
				{
					A[i][j] = 1;// то он инициализируется 1ей
				}
				else
				{
					A[i][j] = _A[i][j];
				}
			}
		
	}



	// далее функция вывода матрицы на консоль
	void print()
	{
		for (int i = 0; i < vertex; i++)
		{
			for (int j = 0; j < vertex; j++)
			{
				std::cout << A[i][j] << "\t";
			}
			std::cout << "\n";
		}
	}




	//вывод на экран массива вершин
	void printV()
	{
		std::cout << "\nВершины:\t";
		for (int i = 0; i < Vsize; i++)
		{
			if(V[i] != 0)
				std::cout << V[i] << "\t";
		}
		std::cout << "\n";
	}
	


	void Verase(int index)
	{
		int* res;
		res = new int[Vsize - 1];

		if (index != 0 && index != Vsize)
		{
			// 1
			for (int i = 0; i < index; i++)
				res[i] = V[i];

			// 2
			for (int i = index + 1; i < Vsize; i++)
				res[i - 1] = V[i];
		}
		else if (index == 0)
		{
			for (int i = 1; i < Vsize; i++)
				res[i - 1] = V[i];
		}
		else
		{
			for (int i = 0; i < Vsize - 1; i++)
				res[i] = V[i];
		}

		delete [] V;
		Vsize--;
		V = new int[Vsize];
		for (int i = 0; i < Vsize; i++)
		{
			V[i] = res[i];
		}

	}



	void setBM(int row, int column, ui vertexVal)//установка значения элемента матрицы А
	{
		A[row][column] = vertexVal;
	}

	int getBM(int row, int column)//возвращает значение из private
	{
		return A[row][column];// возвращает значение элемента матрицы А
	}

	ui getVertex()//возвращает значение из private
	{
		return vertex;// возвращает число вершин (строк и столбцов массива А)
	}

	void operator= (BM b) /*  a = b перегрузка оператора = для выполнения операции
	присваивания между объектами данного класса*/
	{
		vertex = b.vertex; //объект вызывающий оператор =
		for (int i = 0; i < vertex; i++)
		{
			for (int j = 0; j < vertex; j++)
			{
				A[i][j] = b.getBM(i, j);
			}
		}
		// a принимает значения параметров объекта b
	}


	ui And(ui a, ui b)// бинарн. операция конъюкции
	{
		if (a != 0 && b != 0) //1&1 = 1 иначе 0
			return 1;
		else
			return 0;
	}

	ui Or(ui a, ui b)// бинарн. операция дизъюнкции
	{
		if (a == 0 && b == 0)
			return 0;
		else
			return 1;
	}

	BM multBM(BM b) // операция Булевого произведения 2-х матриц названа multBM
	{//использование: Obj.multBM(b) - возвращает обект, содержащий
	//произведение матриц из Obj и b
		ui** Result; // подготовка массива под хранение результата
		/* далее идет проверка необходимых условий для проведения операции
		произведения:
		1. A!=NULL && b.getBM(0,0)!=NULL это условие инициализированности обоих массивов;
		2. vertex == b.getVertex() равенство их числа строк и столбцов.
		 */
		Result = new ui * [vertex];
		for (int i = 0; i < vertex; i++)
		{
			Result[i] = new ui[vertex];//массив готов
		}

		int i, j, k;


		// инициализация массива результата нулями
		for (i = 0; i < vertex; i++)
		{
			for (j = 0; j < vertex; j++)
			{
				Result[i][j] = 0;
			}
		}
		// алгоритм произведения 
		for (i = 0; i < vertex; i++)
		{
			for (j = 0; j < vertex; j++)
			{
				for (k = 0; k < vertex; k++)
				{
					Result[i][j] = Or(And(getBM(i, k), b.getBM(k, j)), Result[i][j]);

				}
			}
		}

		BM c(Result, vertex);
		return c; // возвращает объект, с помощью перегруженного оператора присваивания
	}
	// сложение булевых матриц (объектов одной размерности в смысле числа верш.)
	BM operator+ (BM b)
	{
		int i, j;

		ui** Matr; // для результата расчета
		// далее цикл создания массива
		Matr = new ui * [vertex];
		for (i = 0; i < vertex; i++)
		{
			Matr[i] = new ui[vertex];
		}
		if (b.getVertex() >= getVertex()) {

			for (i = 0; i < vertex; i++)
			{
				for (j = 0; j < vertex; j++)
				{
					// cложение проводится как дизъюнкция
					// В данном классе это функция Or(ui a, ui b)
					Matr[i][j] = Or(getBM(i, j), b.getBM(i, j));
				}
			}
			BM Result(Matr, vertex); // объект для вывода
			return Result;
		}
	}

	// функция возведения булевой матрицы в степень
	BM powBM(ui r)
	{
		/* создание объекта Result для вывода
		и preResult под хранение промежуточн. результата*/
		BM Result(A, vertex);
		BM preResult(A, vertex);
		if (r == 1)
		{
			return Result; //если степень равно 1, то
			//возвращаем без изменений
		}
		if (r == 0) // если степень равно 0, то
		{
			for (int i = 0; i < vertex; i++)
			{
				for (int j = 0; j < vertex; j++)
				{
					Result.setBM(i, j, 1);
				}
			}
			return Result; // возвращаем заполнив единицами
		}
		if (r < 1000)//cтепень не должна превышать 1000
		{
			for (int i = 0; i < r - 1; i++)
			{
				Result = Result.multBM(preResult);
			}
		}
		return Result;
	}

	// функция построения матрицы связности
	BM connectBM()
	{
		int i;
		BM Result1(A, vertex);
		BM preResult(A, vertex);
		BM Result(A, vertex);
		/* возведение булевой матр. в степень с
		последующим суммированием*/
		for (i = 2; i < vertex; i++)
		{
			Result = Result + preResult.powBM(i);
		}
		/*
		теперь производится сложение с единичной матрицей
		*/
		for (i = 0; i < vertex; i++)
		{
			Result.setBM(i, i, 1);
		}
		return Result;
	}

	// функция, возвращающая транспонированную матрицу
	BM transpBM()
	{
		BM Result(A, vertex);//объект результат
		for (int i = 0; i < vertex; i++)
		{
			for (int j = 0; j < vertex; j++)
			{
				Result.setBM(i, j, A[j][i]); //транспонирование
			}
		}
		return Result;
	}

	// булево декартово произведение матриц
	BM operator& (BM b)
	{
		int i, j;
		ui** Res; // для вывода

		Res = new ui * [vertex];

		for (i = 0; i < vertex; i++)
		{
			Res[i] = new ui[vertex];
		}

		// далее алгоритм

		for (i = 0; i < vertex; i++)
		{
			for (j = 0; j < vertex; j++)
			{
				Res[i][j] = And(getBM(i, j), b.getBM(i, j));
			}
		}
		BM Result(Res, vertex);
		return Result;
	}

	// функция построения матрицы cильной связности
	BM powerConnectBM()
	{
		BM Result(A, vertex); // результат
		BM res1(A, vertex); //промежуточный результат 1
		BM res2(A, vertex);//промежуточный результат 2
		res1 = Result.connectBM(); // есть матрица связн.
		res2 = res1.transpBM(); //транспонированная матрица связн.
		Result = res1 & res2;// произведение исходя из формулы
		return Result;
	}

	// функция вычеркивания строки и столбца с номером n
	BM reduceOrderBM(int n)
	{

		ui** res; //матрица для сборки

		res = new ui * [vertex - 1];

		int i, j;

		for (i = 0; i < vertex - 1; i++)
		{
			res[i] = new ui[vertex - 1];
		}




		if (n == 0)
		{
			for (i = 0; i < vertex - 1; i++)
			{
				for (j = 0; j < vertex - 1; j++)
				{
					
					res[i][j] = getBM(i + 1, j + 1);
				}
			}
		}
		else
		{
			//первый цикл перезаписи до n
			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					res[i][j] = getBM(i, j);
				}
			}
			//второй цикл перезаписи 
			for (i = 0; i < n; i++)
			{
				for (j = n + 1; j < vertex; j++)
				{
					res[i][j - 1] = getBM(i, j);
				}
			}
			//3й цикл перезаписи
			for (i = n + 1; i < vertex; i++)
			{
				for (j = n + 1; j < vertex; j++)
				{
					res[i - 1][j - 1] = getBM(i, j);
				}
			}
			//4й цикл перезаписи
			for (i = n + 1; i < vertex; i++)
			{
				for (j = 0; j < n; j++)
				{
					res[i - 1][j] = getBM(i, j);
				}
			}
		}
		BM Result1(res, vertex - 1);

		return Result1;
	}

	// функция выделения компонент сильной связности
	int powConComponent()
	{
		BM Result(A, vertex); // результирующая матрица
		/*далее ведётся подсчет ненулевых элементов
		первой строки. Создаётся набор вершин компоненты
		сильной связности*/
		/* шаг 1. Создаётся матрица сильной связности*/
		Result = Result.powerConnectBM();
		/**/
		int *v; // массив индексов или множество вершин
		// сильной связности
		//подсчет ненулевых элементов:
		int i, j, count = 0, count1 = 0, ver = 0, order = vertex, P = 0;
		/* Р - число компонент cильной
		связности
		order - хранит информацию о текущем порядке матрицы сильн. связн.
		с учетом вычеркиваний*/


		

		while (order > 0)
		{
			for (i = 0; i < order; i++)
			{
				// читается собственная матрица объекта
				// первый индекс 0 соответствует 1 строке
				if (Result.getBM(0, i) > 0) {

					count++;	// количество ненулевых элементов
				}
			}
			if (count == 0)
			{
				order = 0;
				break;
			}
			else
			{
				/* выделение памяти под массив, содержащий
				индексы единичных элементов*/
				v = new int[count];
				count1 = count;
				// запись номеров:
				count = -1; // вспомогательный счетчик
				for (i = 0; i < order; i++)
				{
					if (Result.getBM(0, i) > 0)
					{
						count++;
						v[count] = i;
					}
				}

				std::cout << "\nВершины: ";
				// процедура вычеркивания строк и столбцов
				for(int i = count; i >= 0; i--)
				{
					Result = Result.reduceOrderBM(v[i]);
					std::cout << V[v[i]] << "\t";
					Verase(v[i]);

				}
				
				std::cout << "\nZ" << P + 1 << "(D) = \n";
				Result.print();
				std::cout << "\n";
				
				
				P++;
				order -= count1;/* порядок снизится на count
			т.е. на число вычеркнутых строк и столбцов, соответствующих
			числу вершин*/
				delete[] v;
				count = 0;
			}
		}
		return P;
	}

	~BM()//деструктор класса
	{}

private:
	ui** A;

	int vertex;
};

int main()
{
	setlocale(0, "");
	ui** m;
	int n = 7, i, j;
	

	
	std::cout << "Введите порядок бинарной матрицы:\n";
	std::cin >> n;
	std::cout << "Введите бин. матрицу, нажимая после каждого элемента Enter:\n";
	

	m = new ui * [n];
	for (i = 0; i < n; i++)
	{
		m[i] = new ui[n];
	}

	

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			std::cout << "a" << i + 1 << j + 1 << " = ";
			std::cin >> m[i][j];
		}
		std::cout << "\n";
	}
	

	BM M(m, n);

	BM M1(m, n);

	
	std::cout << "Введённая матрица S(D) =\n";
	M.print();

	//степени матрицы
	for (i = 2; i < n ; i++)
	{
		std::cout << i << "степень: (S(D))^" << i << " =\n";
		M = M1.powBM((ui)i);
		M.print();
		std::cout << "\n\n";
	}

	std::cout << "\nМатрица достижимости: T(D)";
	M = M1.connectBM();
	std::cout << "\n";
	M.print();
	std::cout << "\n\n";

	std::cout << "Матрица сильной связности: Z(D)";
	M = M1.powerConnectBM();
	std::cout << "\n";
	M.print();
	std::cout << "\n\n";

	std::cout << "Вершины компонент сильной связности:\n";
	n = M1.powConComponent();
	std::cout << "Число компонент сильной связности Р = " << n;
	std::cout << "\n\n";



	for (i = 0; i < 3; i++)
	{
		delete[] m[i];
	}
	delete[] m;
	Sleep(2000);
	system("pause");
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
// перезапись массива вершин
// в массив вершин запишется массив y

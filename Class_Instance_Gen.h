
class Shifts
{
public: int day, start, finish;
public:
	Shifts(int d, int s, int f)
	{
		day = d;
		start = s;
		finish = f;
	}
};

class InstanceGenerators
{
public:
	string OriginalPath; //Address where I keep the instances
	string OriginalFileName; //Address where I keep the instances + file name (to read)

	random_device rd;
	mt19937 gen;

	int Number_of_Total_Specialties = 11;
	int Number_Physicians;
	int Number_of_Total_Specialties_In_This_Instance;
	int Number_Physicians_of_Each_Specialty;
	int Patient_Types;

	vector<vector<double>> z; //is Alpha_ss
	vector<vector <Shifts>> Days;
	int G_Setting;
	int H_Setting;

	InstanceGenerators()
	{
		cout << "hello!" << endl;
	}
	~InstanceGenerators() 
	{
		cout << "goodbye!" << endl;
	}

	bool CheckNonRepeatShifts(vector<Shifts>, int);
	bool CheckNonRepeatRef(vector <int>, int);
	bool CheckNonRepeatPath(vector<int> v, int c);
	bool CheckNonConsecut(vector <int> v, int c);
	void Generate_Random_Scenarios(int,int);
	void Write_Instance_File();
	double min_element(vector<double>);
	
};

void InstanceGenerators::Generate_Random_Scenarios(int Number_of_Specialties, int Number_of_Physicians_for_Each_Specialty) {

	Number_of_Total_Specialties_In_This_Instance = Number_of_Specialties;
	Number_Physicians_of_Each_Specialty = Number_of_Physicians_for_Each_Specialty;
	uniform_int_distribution<> dist(1, Number_of_Physicians_for_Each_Specialty);
	vector <int> Rand_Num_Physician;
	int Sumof_Rand=0;
	for (int s = 0;s < Number_of_Total_Specialties_In_This_Instance;s++)
	{
		int Rand = dist(gen);
		Rand_Num_Physician.push_back(Rand);
		Sumof_Rand += Rand;
	}
	for (int s = 0;s < Number_of_Total_Specialties_In_This_Instance;s++)
	{
		cout << Rand_Num_Physician[s]<<"\t";
	}
	Number_Physicians = Sumof_Rand;
	Patient_Types = 2 * Number_of_Specialties;

	vector<int> Physician_Specialty_Ref;
	for (int a = 0; a < Number_of_Total_Specialties_In_This_Instance;)
	{
		int c = rand() % Number_of_Total_Specialties + 1;
		if (CheckNonRepeatRef(Physician_Specialty_Ref, c))
		{
			S_i1_generator.push_back(c);
			for (int b = 0; b < Rand_Num_Physician[a]; b++)
			{
				Physician_Specialty_Ref.push_back(c);
			}
			a++;
		}
	}
	for (int j = 0; j < Number_Physicians; j++)
		cout << Physician_Specialty_Ref[j] << "\t";
	//for (int j = 0; j < Number_Physicians; j++) { //assigning specialties to physicians from piechart
	//	bool key = 0;
	//	int c;
	//	while (key == 0) {
	//		c = rand() % b + 1;
	//		key = CheckNonRepeatRef(Physician_Reference_in_Piechart, c);
	//	}
	//	Physician_Reference_in_Piechart.push_back(c);
	//}

	vector<vector<double>> v(2, vector<double>(Number_Physicians));
	for (int j = 0; j < Number_Physicians; j++)
	{
		v[0][j] = Physician_Specialty_Ref[j];
		v[1][j] = Duration[v[0][j]-1];
	}
	Specialty_and_Duration_of_Physicians = v;
	
	for (int k = 0; k < 2; k++)
	{
		for (int j = 0; j < Number_Physicians; j++)
			cout << Specialty_and_Duration_of_Physicians[k][j] << "\t";
		cout << endl;
	}

	for (int j = 0; j < Number_Physicians; j++)  //computing min & max number of shifts a physician can have
	{
		int a = rand() % 3 + 1;
		int b = rand() % 3 + 1;
		if (a <= b)
		{
			N_j_min = a;
			N_j_max = b;
			N_min.push_back(N_j_min);
			N_max.push_back(N_j_max);
			cout << endl << "N_" << j << "_min = " << N_j_min << "\t" << "N_" << j << "_max = " << N_j_max << endl;
		}
		else
		{
			N_j_min = b;
			N_j_max = a;
			N_min.push_back(N_j_min);
			N_max.push_back(N_j_max);
			cout << endl << "N_" << j << "_min = " << N_j_min << "\t" << "N_" << j << "_max = " << N_j_max << endl;
		}

		K_j = N_j_max + 2;  //computing the number of possible shifts a physician will be available on
		K.push_back(K_j);
		cout << "K_" << j << "= " << K_j << "\t";
		vector <Shifts> temp;
		for (int k = 0; k < K_j;) //obtaining the day, start time and finish time of each possible shift
		{
			int a = rand() % 5 + 1;
			int C[3] = { 240, 300 ,420 };
			int z = rand() % 3;
			C_k = C[z];
			Shift_Duration.push_back(C_k);
			Sum_Shift_Duration += Shift_Duration[k];
			int b = rand() % (((Total_Available_Time - C_k) / 30)+1);
			A_k = b * 30;
			B_k = A_k + C_k;
			if (k == 1 && N_j_min == 2) //if a min # of given shifts is two, a physician's possible shifts must be on at least two days
			{
				if (CheckNonRepeatShifts(temp, a))
				{
					Shifts* test = new Shifts(a, A_k, B_k);
					temp.push_back(*test);
					cout << endl << "Day:" << temp[k].day << "\t";
					cout << "Start:" << temp[k].start << "\t";
					cout << "Finish:" << temp[k].finish << "\n";

					k++;
				}
			}
			else if ((k == 1 || k == 2) && N_j_min == 3) //if a min # of given shifts is three, a physician's possible shifts must be on at least three days
			{
				if (CheckNonRepeatShifts(temp, a))
				{
					Shifts* test = new Shifts(a, A_k, B_k);
					temp.push_back(*test);
					cout << endl << "Day:" << temp[k].day << "\t";
					cout << "Start:" << temp[k].start << "\t";
					cout << "Finish:" << temp[k].finish << "\n";
					k++;
				}
			}
			else
			{
				Shifts* test = new Shifts(a, A_k, B_k);
				temp.push_back(*test);
				cout << endl << "Day:" << temp[k].day << "\t";
				cout << "Start:" << temp[k].start << "\t";
				cout << "Finish:" << temp[k].finish << "\n";
				k++;
			}
		}
		Avg_Shift_Duration.push_back(Sum_Shift_Duration / K_j);
		Days.push_back(temp);
		Shift_Duration.clear();
		Sum_Shift_Duration = 0;

	}

	

	for (int i = 0; i < Patient_Types; i++)  //Creating random pathways for different patient types
	{
		int size = rand() % 5 + 1;
		vector <int> temp;
		for (int l = 0; l < size;)
		{
			int b = rand() % Number_Physicians + 1;
			if (CheckNonRepeatPath(temp, b)&&CheckNonConsecut(temp,b)) //every specialty can be in the path max two times & not consecutively
			{
				temp.push_back(b);
				l++;
			}
		}
		Patient_Pathway.push_back(temp);

	}
	vector<double> Alpha;
	srand(time(NULL));
	for (int i = 0; i < Patient_Types; i++)
	{
		double u;
		u = (double)rand() / (RAND_MAX + 1) + 7 + (rand() % 6);
		double y = u / 10;
		Alpha.push_back(y);
	}
	vector<double> Beta;
	for (int j = 0; j < Number_Physicians; j++)
	{
		double u; 
		u = (double)rand() / (RAND_MAX + 1) + 7 + (rand() % 6);
		double y = u / 10;
		Beta.push_back(y);
	}

	vector<vector<double>> q(Patient_Types, vector<double>(Number_Physicians));
	for (int i = 0; i < Patient_Types; i++)
	{
		for (int j = 0; j < Number_Physicians; j++)
		{
			
			double x=Beta[j] * Specialty_and_Duration_of_Physicians[1][j]*Alpha[i];
			q[i][j]=x;

		}
		
	}
	M_i_j = q;
	/*for (int i = 0; i < Patient_Types; i++)
	{
		for (int j = 0; j < Number_Physicians; j++)
		{
			cout << M_i_j[i][j]<<"\t\t";
		}
		cout << endl;
	}*/

	vector<vector<double>> zz;
	vector<double> zzz;
	for (int i = 0; i < Patient_Types; i++)
	{
		for (int k = 0; k < Number_of_Total_Specialties; k++)
		{
			for (int l = 0; l < Number_of_Total_Specialties; l++)
			{
				
				double a = z[k][l] * (double)rand() / (RAND_MAX + 1);
				zzz.push_back(a);
			}
			zz.push_back(zzz);
			zzz.clear();
		}
		Alpha_iss_initial.push_back(zz);
		zz.clear();
	}
for (int l = 0; l < Number_of_Total_Specialties; l++)
	{
		for (int k = 0; k < Number_of_Total_Specialties; k++)
		{
			double makhraj = 0;
			for (int i = 0; i < Patient_Types; i++)
			{
				makhraj += Alpha_iss_initial[i][k][l];
			}
			for (int i = 0; i < Patient_Types; i++)
			{
				if (makhraj != 0)
					Alpha_iss_initial[i][k][l] = Alpha_iss_initial[i][k][l] * z[k][l] / makhraj;
				else
					Alpha_iss_initial[i][k][l] = 0;
			}
		}
	}
	double Sum_r = 0;
	vector<double> r;
	for (int i = 0; i < Patient_Types; i++)
	{
		double u = (double)rand() / (RAND_MAX + 1) + 7 + (rand() % 6);
		double y = u / 10;
		r.push_back(y);
		Sum_r+= r[i];
	}
	
	vector<vector<double>> u(Patient_Types, vector<double>(Number_of_Total_Specialties));
	for (int i = 0; i < Patient_Types; i++)
	{
		for (int s = 0; s < Number_of_Total_Specialties; s++)
			u[i][s] = r[i] / Sum_r;
	}
	U_i_s = u;
	//for (int i = 0; i < Patient_Types; i++)
	//{
	//	for (int s = 0; s < Number_of_Total_Specialties; s++)
	//		cout << U_i_s[i][s] << "\t";
	//}
	vector<int> How_many_of_one_specialty;
	for (int s = 0; s < Number_of_Total_Specialties; s++)
	{
		int a = 0;
		for (int j = 0; j < Number_Physicians; j++)
		{
			if (Specialty_and_Duration_of_Physicians[0][j] == s + 1)
				a++;
		}
		How_many_of_one_specialty.push_back(a);
	}
	//for (int s = 0; s < Number_of_Total_Specialties; s++)
	//	cout << How_many_of_one_specialty[s] << "\t";

	vector<int>test;
	vector<vector<vector<double>>> xx(Patient_Types, vector<vector<double>>(Number_of_Total_Specialties,vector<double>(Number_Physicians)));
	for (int i = 0; i < Patient_Types; i++)
	{
		int a = 0; 
		for (int s = 0; s < Number_of_Total_Specialties; s++)
		{
			for (int j = 0; j < Number_Physicians; j++)
			{
				if (Specialty_and_Duration_of_Physicians[0][j] == s + 1)
					a++;
			}

			test.push_back(a);
		}
		for (int s = 0; s < Number_of_Total_Specialties; s++)
		{
			for (int j = 0; j < Number_Physicians; j++)
			{
				
					xx[i][s][j] = U_i_s[i][s] * N_max[j] * Avg_Shift_Duration[j] * a / M_i_j[i][j];
				
			}
		}
	}
	for (int s = 0; s < Number_of_Total_Specialties; s++)
		cout << test[s] << endl;
	//for (int i = 0; i < Patient_Types; i++)
	//{
	//	for (int s = 0; s < Number_of_Total_Specialties; s++)
	//	{
	//		for (int j = 0; j < Number_Physicians; j++)
	//			cout << xx[i][s][j] << "\t";
	//	}
	//	cout << endl;
	//}

	vector<double> Min(Patient_Types);
	for (int i = 0; i < Patient_Types; i++)
	{
		vector<double> Min_2d(Number_of_Total_Specialties);
		for (int s = 0; s < Number_of_Total_Specialties; s++)
		{
			Min_2d[s] = min_element(xx[i][s]);
		}
		Min[i] = min_element(Min_2d);
	}
	//for (int i = 0; i < Min.size(); ++i)
	//{
	//	cout << Min[i] << endl;
	//}
	M_i = Min;
	for (int i = 0; i < Patient_Types; i++)
	{
		int a = rand() % Number_of_Total_Specialties_In_This_Instance;
		S_i1.push_back(S_i1_generator[a]);
	}
	for (int i = 0; i < Patient_Types; i++)
		cout << S_i1[i]<<"\n";

	for (int i = 0; i < Patient_Types; i++)
	{
		C_importance.push_back((rand() % 10) + 1);
	}
}

double InstanceGenerators::min_element(vector<double> v)
{
	double temp = v[0];
	for (int i = 0; i < v.size(); i++)
	{
		if (temp > v[i])
			temp = v[i];
	}
	return temp;
}
bool InstanceGenerators::CheckNonRepeatShifts(vector<Shifts> v, int c) {
	
	for (int j = 0; j < v.size(); j++)
	
		if (v[j].day == c)
			return false;
	return true;
}
bool InstanceGenerators::CheckNonRepeatRef(vector <int> v, int c) {
	for (int j = 0; j < v.size(); j++)
	{
		if (v[j] == c)
			return false;
	}
	return true;
}
bool InstanceGenerators::CheckNonRepeatPath(vector <int> v, int c) {
	int a = 0;
	for (int j = 0; j < v.size(); j++)
	{
		if (v[j] == c)
		{
			a++;
			if (a == 2)
				return false;
		}
	}
	return true;
}
bool InstanceGenerators::CheckNonConsecut(vector <int> v, int c) {
	if (v.size()!=0 && v[v.size()-1] == c)
		return false;
	return true;
}
void InstanceGenerators::Write_Instance_File() {
	OriginalPath = ARG1;//OriginalPath is the path to read the instance file, I have defined a global variable to this class so that in other functions of this class I can call it if I need.
	stringstream ss;
	stringstream aa;
	//Write a simple code to update aa to be the name of your instance using the input arguments (only the name, it does not include the address of the folder where it is located). I have provided an example in the next line:
	aa << "Instances_" << Number_of_Specialties << "_" << Number_of_Physicians_for_Each_Specialty << "_" << Current_Instance_Number << ".data";
	OriginalFileName = aa.str();//OriginalFileName includes the name of the file to read. aa.str() just changes aa to string.
	ss << OriginalPath;
	ss << OriginalFileName;
	string TheAddressInString = ss.str();//TheAddressInString includes the address and the name of the file to read. ss.str() just changes ss to string.
	string Instances = TheAddressInString;
	ofstream filename(Instances.c_str());
	if (filename.is_open()) {
		filename << Total_Available_Time << endl;
		filename << Number_Physicians << endl; 
		filename << Patient_Types << endl;
		filename << Number_of_Total_Specialties <<endl;
		filename << G_Setting << endl;
		filename << H_Setting << endl << endl;

		
		for (int j = 0; j < Number_Physicians; j++)
			filename << Specialty_and_Duration_of_Physicians[0][j] <<  "\t" << N_min[j] << "\t" << N_max[j] << endl;
		
			
		filename << endl;
		for (int j = 0; j < Number_Physicians; j++)
			filename << K[j]<<endl;
		filename << endl;
		for (int j = 0; j < Number_Physicians; j++)
		{
			for (int k = 0; k < Days[j].size(); k++)
				filename <<Days[j][k].day << "\t" << Days[j][k].start << "\t" << Days[j][k].finish << endl;
			filename << endl;
		}
		for (int i = 0; i < Patient_Types; i++)
			filename << M_i[i] << "\t";
		filename << endl<<endl;
		for (int i = 0; i < Patient_Types; i++)
		{
			for (int j = 0; j < Number_Physicians; j++)
			{
				filename << M_i_j[i][j] << "\t\t";
			}
			filename << endl;
		}
		filename << endl;
		for (int i = 0; i < Patient_Types; i++)
		{
			for (int k = 0; k < Number_of_Total_Specialties; k++)
			{
				for (int l = 0; l < Number_of_Total_Specialties; l++)
				{
					filename << floor(1000*Alpha_iss_initial[i][k][l])/1000 << "\t";
				}
				filename << endl;
			}
			filename << endl;
			filename << endl;
		}
		filename << endl;

		for (int i = 0; i < Patient_Types; i++)
		{
			filename << S_i1[i] << endl;
		}
		filename << endl;
		filename << endl;

		for (int i = 0; i < Patient_Types; i++)
		{
			filename << C_importance[i] << endl;
		}

		filename.close(); 
		Physician_Reference_in_Piechart.clear();
		N_min.clear();
		N_max.clear();
		K.clear();
		Days.clear();
		Patient_Pathway.clear();
		M_i.clear();
		M_i_j.clear();
		Alpha_iss_initial.clear();
		S_i1.clear();
		S_i1_generator.clear();
		C_importance.clear();
	}
	else {
		cout << "Unable to open file to write results" << endl;
		system("Pause");
	}
}


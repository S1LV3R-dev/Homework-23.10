#include <iostream>
#include <fstream>
#include <ctime>

bool generate_file(int N, const char* fname) {
    std::ofstream outFile(fname);
    if (!outFile) return false;
    while (N-- > 0)
        outFile << ' ' << rand() % 10000;
    outFile.close();
    return true;
}

bool isSortedFile(const char* fname) {
    std::ifstream inFile;
    inFile.open(fname);
    if (!inFile)
        throw "Can not open file";
    int last, current;
    bool answer = true;
    inFile >> last;
    while (inFile.peek() != EOF) {
        inFile >> current;
        if (current < last) {
            answer = false;
            break;
        }
        last = current;
    }
    inFile.close();
    return answer;
}

bool _split_file(const char *fname, const char *tmp1, const char *tmp2, const char *tmp3) {
    std::ifstream inFile(fname);
    if (!inFile) throw "Can not read file";

    std::ofstream outFile[3];

    outFile[0].open(tmp1);
    if (!outFile[0]) throw "Can not write to file";

    outFile[1].open(tmp2);
    if (!outFile[1]) throw "Can not write to file";

    outFile[2].open(tmp3);
    if (!outFile[2]) throw "Can not write to file";

    bool result = false;
    int current, last;
    unsigned char num = 0;
    inFile >> last;
    outFile[num] << last;
    while (inFile.peek() != EOF) {
        inFile >> current;
        if (current < last) {
            num++;
            num%=3;
            result = true;
        }
        outFile[num] << ' ' << current;
        last = current;
    }
    inFile.close(); outFile[0].close(); outFile[1].close();
    return result;
}

void _merge_file(const char* tmp1, const char* tmp2, const char* tmp3, const char *fname) {
    std::ofstream outFile(fname);
    if (!outFile) throw "Can not write to file";

    std::ifstream inFile[3];
    inFile[0].open(tmp1);
    if (!inFile[0]) throw "Can not read file tmp1";
    inFile[1].open(tmp2);
    if (!inFile[1]) throw "Can not read file tmp2";
	inFile[2].open(tmp3);
    if (!inFile[2]) throw "Can not read file tmp3";
    unsigned char num=0;
	int last, current[3]={0};
    inFile[0] >> current[0];
    if(inFile[1].peek()!=EOF) inFile[1] >> current[1];
    if(inFile[2].peek()!=EOF)
	{
		inFile[2] >> current[2];
	    num=0;
		num = (current[0] < current[1] && current[0]<current[2]) ? 0 : ((current[1] < current[0] && current[1]<current[2]) ? 1 : 2);
	    outFile <<' '<< current[num]; last = current[num];
	    while (inFile[num].peek() != EOF)
		{
	        inFile[num] >> current[num];
	        bool s1 = current[0] >= last;
	        bool s2 = current[1] >= last;
			bool s3 = current[2] >= last;
			
	        if (s1 == s2 && s1==s3)
			{
	        	num = (current[0] < current[1] && current[0]<current[2]) ? 0 : ((current[1] < current[0] && current[1]<current[2]) ? 1 : 2);
	        } 
			else
			{
				if (s1 && s2)
				{
					num = current[0] < current[1] ? 0 : 1;
				}
				else if (s2 && s3)
				{
					num = current[1] < current[2] ? 1 : 2;
				}
				else if(s1 && s3)
				{
					num = current[0] < current[2] ? 0 : 2;
				}
				else
				{
					num = s1 ? 0 : (s2 ? 1 : 2);
				}
	        }
	        outFile << ' ' << current[num];
	        last = current[num];
	    }
	}
	else num=2;
	unsigned char k=(num+1)%3,l=(num+2)%3;
    num = (current[k] < current[l]) ? k : l;
    outFile <<' '<< current[num]; last = current[num];
    while (inFile[num].peek() != EOF)
	{
        inFile[num] >> current[num];
        bool s1 = current[num] >= last;
		unsigned char ch;
		if(num==k)
			ch=l;
		else
			ch=k;
		bool s2 = current[ch] >= last;
        if (s1 == s2) {
            num = (current[k] < current[l]) ? k : l;
        } 
		else 
		{
			if(!s1)
			{
				num=ch;
			}
        }
        outFile << ' ' << current[num];
        last = current[num];
    }
    if(num==l) num=k;
    else num=l;
    outFile <<' '<< current[num];
    while (inFile[num].peek() != EOF)
	{
        inFile[num] >> last;
        outFile << ' ' << last;
    }
    outFile.close(); inFile[0].close(); inFile[1].close(); inFile[2].close();
}

int MergeFileSorting(const char* fname, const char * outfilename) {
    char tmp1[] = "_temp_file_1.txt", tmp2[] = "_temp_file_2.txt", tmp3[] = "_temp_file_3.txt";
    bool key;
    int counter = 0;
    key = _split_file(fname, tmp1, tmp2, tmp3);
    while (key) {
    	_merge_file(tmp1, tmp2, tmp3, outfilename);
        key = _split_file(outfilename, tmp1, tmp2, tmp3);
        ++counter;
    } 
    return counter;
}


int main()
{
    clock_t tictok = clock();

    char fname[] = "data.txt";
    char outfname[] = "result.txt";
    if (!generate_file(10, fname)) {
        std::cout << "Can not create file for test!" << std::endl;
        return 1;
    }

    int N=MergeFileSorting(fname, outfname);

    std::cout << clock() - tictok << " tics" << std::endl;

    try {
        if (isSortedFile(outfname)) {
            std::cout << "File " << outfname << " is sorted." << std::endl;
            std::cout << "Total " << N << " splits." << std::endl;
        }
        else {
            std::cout << "File " << outfname << " is not sorted." << std::endl;
        }
    }
    catch (const char* error) {
        std::cout << "Error: " << error << std::endl;
    }

    return 0;
}

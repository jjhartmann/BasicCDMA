#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>


using namespace std;

// Basic class to implement CDMA functionality
class CDMA
{
public:
    // Constructor
    CDMA(string filename)
    {
        ifstream inFile(filename);

        if (!inFile)
            return;

        string line;

        // Get number of users. 1- 8
        getline(inFile, line);
        m_users = atoi(line.substr(0,1).c_str());

        // Get code length max 16
        getline(inFile, line);
        m_codelength = atoi(line.substr(0, 2).c_str());

        // Get data bits
        getline(inFile, line);
        m_databits = atoi(line.substr(0,1).c_str());

        // Get Users Codes
        for (int i = 0; i < m_users; ++i)
        {
            getline(inFile, line);
            extractData(m_codelength, line, m_userCode);
        }

        // Get user bits
        // Get Users Codes
        for (int i = 0; i < m_users; ++i)
        {
            getline(inFile, line);
            extractData(m_databits, line, m_userBits);
        }
    }

    // Destructor
    ~CDMA()
    {}


    // Check if the codes are orthogonal to each other.
    // Return false if the codes are not orthogonal
    bool CheckOrthogonality()
    {
        int iters = choose2(m_users);

        int j = 0, k = 1, tracker = 0;
        for (int i = 0; i < iters; ++i)
        {

            if (dotProduct(j, k))
            {
                return false;
            }


            if (0 == ((m_users - 1) - j))
            {
                ++tracker;

                j = tracker;
                k = tracker + 1;
            }

            ++j;
            ++k;
        }

        return true;
    }

    // Compute bits
    void EncodeBits()
    {
        // Create the encoded bits
        for (int i = 0; i < m_users; ++i)
        {
            // The bits
            for (int j = 0; j < m_databits; ++j)
            {
                // The code
                for (int k = 0; k < m_codelength; ++k)
                {
                    m_encodedBits.push_back(m_userCode[i*m_codelength + k] * m_userBits[i*m_databits + j]);
                }
            }
        }

        // Add the bits to create interference pattern
        // The bits
        for (int j = 0; j < m_databits; ++j)
        {
            for (int k = 0; k < m_codelength; ++k)
            {
                int temp = 0;
                for (int i = 0; i < m_users; ++i)
                {
                        temp += m_encodedBits[i*m_codelength*m_users + m_codelength*j + k];
                }

                m_resultingBits.push_back(temp);
            }

        }
    }

    // Print the results
    void Print()
    {
        for (int i = 0; i < m_databits; ++i)
        {
            for (int j = 0; j < m_codelength; ++j)
            {
                cout << m_resultingBits[i*m_codelength + j]  << " ";
            }

            cout << endl;
        }


    }


    // Private Functions
private:
    // Dot Product
    int dotProduct(int v1, int v2)
    {
        int res = 0;
        for (int i = 0; i < m_codelength; ++i)
        {
            res += m_userCode[v1*m_codelength + i] * m_userCode[v2*m_codelength + i];
        }

        return res;
    }


    // Simple Choose2 Function
    int choose2(int in_n)
    {
        if (in_n == 2)
            return 1;

        int n = 1;
        for (int i = in_n; i > 2; --i)
        {
            n = n * i;
        }

        int n2 = 1;
        for (int i = in_n - 2; i > 0; --i)
        {
            n2 = n2 * i;
        }

        return n / (n2 *2);
    }


    // Extract the code and bit information from file
    void extractData(int len, string code, vector<int> &user)
    {
        int previndex = 0;
        int startindex = 0;
        int strlen = code.length();

        for (int i = 0; i < len; ++i)
        {
            startindex = code.find(' ', startindex + 1);

            if (startindex == string::npos)
            {
                startindex = strlen;
            }

            int temp = atoi(code.substr(previndex, startindex - previndex).c_str());
            user.push_back(temp);

            previndex = startindex;
        }
    }

    // Variables
private:
    int m_users;
    int m_codelength;
    int m_databits;
    vector<int> m_userCode; // Have mutliple users use array
    vector<int> m_userBits;
    vector<int> m_encodedBits;
    vector<int> m_resultingBits;


};




int main()
{
    string filename;
    // TUI
    // Ask user for name of text file with test inforamtion.
    cout << "Provide the test file you wish to use (ie filename.txt): " << endl;
    cin >> filename;


    // Open the Text file and build initial CDMA state.
    CDMA cdma(filename);

    // Basic CDMA
    // 1. Check whether codes are orthogonal to eachother.
    // IF YES, move to 2.
    if (!cdma.CheckOrthogonality())
    {
        // The code is not orthogonal.
        return 0;
    }

    // 2. Start building bits for combined codes.
    cdma.EncodeBits();


    // 3. Print resutls
    cdma.Print();




    cout << "Finsih" << endl;
    return 0;
}
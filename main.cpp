#include <iostream>
#include <algorithm>
using namespace std;

int strLen(char* str)
{
    int i = 0;
    while(str[i] != 0) i++;
    return i;
}

//to map any char to it's rank(order)
class charMapper
{
    private:
        //to hold any symbol(char) with rank
        int arr[256];
    public:

        void put(char c, int rnk)
        {
            arr[(int)c] = rnk;
        }

        int get(char c)
        {
            return arr[(int)c];
        }
};

struct suffixNode
{
    int ind = -1, r1 = -1, r2 = -1;

    static bool indComp(suffixNode a, suffixNode b)
    {
        return a.ind < b.ind;
    }

    static bool comp(suffixNode a, suffixNode b)
    {
        if(a.r1 != b.r1) return a.r1 < b.r1;
        return a.r2 < b.r2;
    }
};


class SuffixArray
{
    private:

        suffixNode* arr;
        int* suffix;
        int sz;

    public:

        SuffixArray(char str[])
        {
            char* sortedCpy = new char[strLen(str)+1];
            sz = strLen(str);
            sortedCpy[sz] = 0; //null character
            for(int i=0;i<sz;i++) sortedCpy[i] = str[i];
            sort(sortedCpy, sortedCpy+sz);
            charMapper* mp = new charMapper;
            int i = 0, rnk = 0;
            while(i < sz)
            {
                mp->put(sortedCpy[i], rnk++);
                int j = i+1;
                while(j < sz && sortedCpy[j] == sortedCpy[i]) j++;
                i = j;
            }
            //create suffix and array to the size of string
            arr = new suffixNode[sz];
            suffix = new int[sz];
            //filling r1 and ind info
            for(int i=0;i<sz;i++)
            {
                int order = mp->get(str[i]);
                //cout<<"order = "<<order<<" for "<<str[i]<<endl;
                arr[i].r1 = order;
                arr[i].ind = i;
            }
            //filling r2 info
            for(int i=0;i<sz-1;i++) arr[i].r2 = arr[i+1].r1;
            delete mp;
        }

        void ConstructUsingPrefixDoubling()
        {
            for(int j=1;(1<<j)<sz;j++)
            {
                sort(arr, arr+sz, suffixNode::comp);
                int newOrder = 0, i = 0;
                while(i < sz)
                {
                    int r1 = arr[i].r1, r2 = arr[i].r2;
                    int j = i+1;
                    while(j < sz && arr[j].r1 == r1 && arr[j].r2 == r2) j++;
                    for(int t=i;t<j;t++) arr[t].r1 = newOrder;
                    i = j;
                    newOrder++;
                }
                sort(arr, arr+sz, suffixNode::indComp);
                //update r2
                for(int i=0;i<sz-(1<<j);i++) arr[i].r2 = arr[i+(1<<j)].r1;
                //if it has no new r2 update it to be -1
                for(int i=sz-(1<<j);i<sz;i++) arr[i].r2 = -1;
            }
            //fill our suffix array
            for(int i=0;i<sz;i++) suffix[arr[i].r1] = i;
        }

        void Print()
        {
            for(int i=0;i<sz;i++) cout<<suffix[i]<<' ';
            cout<<endl;
        }
};

int main()
{
    SuffixArray t("ACGACTACGATAAC$");
    t.ConstructUsingPrefixDoubling();
    t.Print(); // Prints:  14 11 12  0  6  3  9 13  1  7  4  2  8 10  5
    return 0;
}

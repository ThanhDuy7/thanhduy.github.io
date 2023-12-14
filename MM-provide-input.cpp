#include<iostream>
#include<random>
#include<chrono>
#include<limits>
using namespace std;

int MAXINT=abs(numeric_limits<int>::max());

struct Arc
{
    int left,right,cost,cap;
};
Arc set(int l,int r,int c,int cap)
{
    Arc s;
    s.left=l;
    s.right=r;
    s.cost=c;
    s.cap=cap;
    return s;
}

int main()
{
    int n,m,x,y,p;
    int cost_range;
    bool *nodelist;
    int *source,*sink,*demand;
    int **net;
    Arc *netlist;
    int *check;
    unsigned generator = chrono::system_clock::now().time_since_epoch().count();
    mt19937 random(generator);


    cout<<"Insert the number of Nodes:";
    cin>>n;
    nodelist=new bool[n];
    for(int it=0;it<n;it++) nodelist[it]=false;
    demand=new int[n];
    for(int it=0;it<n;it++) demand[it]=0;
    check=new int[n];
    for(int it=0;it<n;it++) check[it]=0;
    uniform_int_distribution<int> create_short(1,n/3);
    uniform_int_distribution<int> create_long(0,n-1);

    cout<<"Insert the number of people:";
    cin>>p;

    cout<<"Insert the range of cost:";
    cin>>cost_range;
    uniform_int_distribution<int> create_cost(1,cost_range);


    x=create_short(random);
    //cout<<x;
    source=new int[x];
    int source_demand=p;
    for(int it=0;it<x;it++)
    {
        int s=create_long(random);
        while(nodelist[s]) s=create_long(random);
        nodelist[s]=true;
        check[s]=1;
        source[it]=s;
        uniform_int_distribution<int> create_source_demand(1,source_demand/(x-it));
        int d=create_source_demand(random);
        demand[s]=d;
        source_demand-=d;
    }
    if(source_demand>0) demand[source[x-1]]+=source_demand;



    y=create_short(random);
    sink=new int[y];
    int sink_demand=p;
    for(int it=0;it<y;it++)
    {
        int s=create_long(random);
        while(nodelist[s]) s=create_long(random);
        nodelist[s]=true;
        check[s]=-1;
        sink[it]=s;
        uniform_int_distribution<int> create_sink_demand(1,sink_demand/(y-it));
        int d=create_sink_demand(random);
        demand[s]=-d;
        sink_demand-=d;
    }
    if(sink_demand>0) demand[sink[y-1]]-=sink_demand;



    net=new int*[n];
    for(int it=0;it<n;it++) net[it] = new int[n];

    uniform_int_distribution<int> create_capacity(p/2, p*3/2);
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++) net[i][j] = 0;
    m=(n-1)*n/2-x*y;
    netlist=new Arc[m];
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++) net[source[i]][sink[j]]=-1;
    bool **visit;
    visit=new bool*[n];
    for(int i=0;i<n;i++)
    {
        visit[i]=new bool[n];
        for (int j=0;j<n;j++) visit[i][j]=false;
    }
    int vs=0;
    bool c=false;
    int flag=-1;
    for(int i=0;i<m;i++)
    {
        int u=create_long(random);
        int v=create_long(random);

        while (u==v||net[u][v]!=0||net[v][u]!=0||check[v]==1||check[u]==-1)
        {
            if(check[v]==1||check[u]==1)
            {
                if(visit[u][v]) vs++;
                else visit[u][v]=true;
                if(visit[v][u]) vs++;
                else visit[v][u]=true;
            }
            u=create_long(random);
            v=create_long(random);
            if(visit[u][v]) vs++;
            else visit[u][v]=true;
            if(vs>=n*n)
            {
                flag=i;
                c=true;
                break;
            }
        }

        if(c) break;
        int capacity=create_capacity(random);
        int cost=create_cost(random);
        net[u][v]=capacity;
        netlist[i]=set(u,v,cost,capacity);
    }



    cout<<"The number of Disaster areas:"<<x<<endl;
    for(int it=0;it<x;it++) cout<<"Node:"<<source[it]<<" "<<"Demand:"<<demand[source[it]]<<endl;

    cout<<"The number of Safe areas:"<<y<<endl;
    for(int it=0;it<y;it++) cout<<"Node:"<<sink[it]<<" "<<"Demand:"<<demand[sink[it]]<<endl;


    if(c) m=flag;
    cout<<"The number of Arcs:"<<m<<endl;
    for(int i=0;i<m;i++) cout<<"Link:"<<netlist[i].left<<" -> "<<netlist[i].right<<" Cost:"<<netlist[i].cost<<" Cap:"<<netlist[i].cap<<endl;


    delete[] nodelist;
    delete[] source;
    delete[] sink;
    delete[] demand;
    for (int it = 0; it < n; it++)
        delete[] net[it];
    delete[] net;




    cout<<endl;
    cout<<"NOTE: This example can lead to a infeasible min-cost flow problem."<<endl;
    return 0;

}

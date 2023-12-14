#include<vector>
#include<queue>
#include<limits>
#include<algorithm>
#include<iostream>

using namespace std;
int MAXINT=abs(numeric_limits<int>::max());


class Arc
{
private:
    bool d;
public:
    int left;
    int right;
    int cap;
    int cost;
public:
    Arc()
    {
        left=right=cap=cost-1;d=false;
    }
    Arc(int l,int r,int cost,int cap)
    {
        left=l;
        right=r;
        this->cost=cost;
        this->cap=cap;
        d=true;
    }
    void activate()
    {
        d=true;
    }
    void deactivate()
    {
        d=false;
    }
    bool check()
    {
        if(cap>0) return d;
        else return false;
    }
};
class SuccessiveShortestPathFlowNetwork
{
public:
    vector<vector<Arc>> net;
    /*
    net[i][j] contain an Arc connect Node i to Node j
                            */
    vector<vector<int>> city;
    /*
    city[i] is a vector contain a list of Nodes that Node i connected to
                            */

    vector<vector<int>> flow_net;
    /*
    flow_net[i][j] contain the flow of the Arc(i,j)
    This net is for showing the specific track
                            */
    int total_demand;
    int total_accept;

    int supersource;        //index of supersource
    int supersink;          //index of supersink



public:
    void create_city(int n)
    {
        vector<vector<int>> temp1(n+3);
        vector<vector<Arc>> temp2(n+3,vector<Arc>(n+3));
        vector<vector<int>> temp3(n+3,vector<int>(n+3,0));

        city=temp1;
        net=temp2;
        flow_net=temp3;


        for(int i=0;i<net.size();i++)
            for(int j=0;j<net.size();j++) net[i][j].deactivate();

        supersource=n+1;
        supersink=n+2;

        total_demand=0;
        total_accept=0;
    }
    void add_arc(int left, int right, int cost, int cap)
    {
        net[left][right]=Arc(left,right,cost,cap);
        city[left].push_back(right);
    }
    void add_demand(int demand,int index)
    {
        if(demand>0)
        {
            total_demand+=demand;
            city[supersource].push_back(index);
            net[supersource][index]=Arc(supersource,index,0,demand);
        }
        else
        {
            total_accept+=demand;
            city[index].push_back(supersink);
            net[index][supersink]=Arc(index,supersink,0,-demand);
        }
    }
    int min_cost_flow()
    {
        int cost=0;
        while(total_accept<0&&total_demand>0)
        {


        //***************** Step 2: Dijkstra start *****************//
            vector<int> dist(net.size(),MAXINT);    //distance array
            vector<int> prev(net.size(),-1);        //recursive array
            vector<bool> visit(net.size(),false);   //tracking array

            dist[supersource]=0;

            priority_queue<pair<int,int>> q;
            q.push({0,supersource});

            while(!q.empty())
            {
                int l=q.top().second;
                q.pop();
                if(visit[l]) continue;
                visit[l]=true;
                for(int r:city[l])
                {
                    Arc arc=net[l][r];
                    if(arc.check()&&dist[l]+arc.cost<dist[r])
                    {
                        dist[r]=dist[l]+arc.cost;
                        prev[r]=l;
                        q.push({-dist[r],r});
                    }
                }
            }
            if(dist[supersink]==MAXINT)
            {
                break;
            }
            //***************** Step 2: Dijkstra finish *****************//


            //***************** Step 3: find the max flow. start ****************//
            int max_flow=MAXINT;
            for(int p=supersink;prev[p]!=-1;p=prev[p])
            {
                max_flow=min(max_flow,net[prev[p]][p].cap);
            }



            for(int p=supersink;prev[p]!=-1;p=prev[p])
            {
                Arc &arc=net[prev[p]][p];

                int r=arc.right;
                int l=arc.left;

                arc.cap-=max_flow;
                if(arc.cost>0)
                    flow_net[l][r]+=max_flow;
                else
                    flow_net[r][l]-=max_flow;

                //Create reverse Arc: start
                net[r][l]=Arc(r,l,-arc.cost,max_flow);
                city[r].push_back(l);
                //Create reverse Arc: end

                if(arc.cap<=0)
                    arc.deactivate();                   //The arc is no longer available
                if(net[r][l].cap<=0)
                    net[r][l].deactivate();             //The arc is no longer available
            }
            //***************** Step 3: find the max flow. end ****************//

            //****Step 4: start ****//
            total_demand-=max_flow;
            total_accept+=max_flow;
            cost+=dist[supersink]*max_flow;
            //****Step 4: end   ****//
        }

    if(total_demand>0) return -1;           //still people left behind

    return cost;                            //the algorithm succeed
    }
};

int main()
{
    SuccessiveShortestPathFlowNetwork graph;

    int n;
    cout<<"The number of Nodes:";
    cin>>n;
    graph.create_city(n);

    int m;
    cout<<"The number of Arcs:";
    cin>>m;
    for(int i=1;i<=m;i++)
    {
        int l,r,cost,cap;
        cin>>l>>r>>cost>>cap;
        graph.add_arc(l,r,cost,cap);
    }

    int x;
    cout<<"The number of disaster areas:";
    cin>>x;
    for(int i=1;i<=x;i++)
    {
        int index,demand;
        cin>>index>>demand;
        graph.add_demand(demand,index);
    }

    cout<<"The number of safe areas:";
    int y;
    cin>>y;
    for(int i=1;i<=y;i++)
    {
        int index,demand;
        cin>>index>>demand;
        graph.add_demand(-demand,index);
    }

    cout<<"Min cost:";
    cout<<graph.min_cost_flow()<<endl;
    //graph.print();
}

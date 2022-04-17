#include<bits/stdc++.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#define isValid(i,j) (i>=0 && i<ROW &&j>=0 && j<COL)


using namespace std;

int ROW;
int COL;
int FLOWERCOUNT;
int COVERCOUNT = ROW*COL;
bool GAMEOVER = false;
bool WIN = false;


bool isNotInSquare(int x, int y, int i, int j)
{
    bool isNot = true;
    int p,q;
    for(p=i-1;p-i+1<3;p++)
    {
        for(q=j-1;q-j+1<3;q++)
        {
            if(isValid(p,q))
            {
                if(p==x && q==y)
                {
                    isNot = false;
                }
            }
        }
    }
    return isNot;
}
//Structure for FLOWER
struct FLOWER
{
    int x;
    int y;
};

//Game goes here
class GameBoard
{
    vector< vector<int> > mines;//Matrix to store EPMTY SPACE = 0, FLOWER = -1, and NUMBERS>0 = no. of FLOWERS around it
    vector< vector<int> > isCovered;//Matrix to store 0 = NOR COVERED, 1 = COVERED
    vector<FLOWER> FLOWERVector; //Stores exact location of FLOWERs in an array of FLOWER.
    void setUpBoards();//resize and fill in the initial values of mines[] and isCovered
    void placeFLOWERs(int a, int b);//place FLOWERs randomly on mines[] board, except in the 9x9 square centred at the point of first click (a,b)
    void placeNumbers();//place numbers to show where the FLOWERs are, on mines[].
    void unCover(int i, int j);//Uncovers tiles when clicked by user; for the first click, call placeFLOWERs and placeNumbers
public:
    GameBoard();
    void printBoard();
    void askUser();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameBoard::setUpBoards()
{
    int i,j;
    mines.resize(ROW);
    isCovered.resize(ROW);
    //Initializee mines[]
    for(i=0;i<ROW;i++)
    {
        mines[i].resize(COL);
        for(j=0;j<COL;j++)
        {
            mines[i][j] = 0;
        }
    }
    //Initialize isVisited[]
    for(i=0;i<ROW;i++)
    {
        isCovered[i].resize(COL);
        for(j=0;j<COL;j++)
        {
            isCovered[i][j] = 1;
        }
    }
}
//Place FLOWERs randomly on board
void GameBoard::placeFLOWERs(int a,int b)
{
    srand(time(NULL));
    FLOWERVector.resize(FLOWERCOUNT);
    set< pair<int,int> > FLOWERSet;
    int i,p,q;
    for(i=0;i<FLOWERCOUNT;i++)
    {
        do
        {
            p = rand()%ROW;
            q = rand()%COL;
        }while(!isNotInSquare(p,q,a,b) || !(FLOWERSet.find( pair<int,int> (p,q))==FLOWERSet.end()));

        FLOWERVector[i].x = p;
        FLOWERVector[i].y = q;
        FLOWERSet.insert( pair<int,int> (p,q));
    }

    for(i=0;i<FLOWERCOUNT;i++)
    {
        mines[FLOWERVector[i].x][FLOWERVector[i].y] = -1; //Place -1 in board where FLOWER lies.
    }
}

void GameBoard::placeNumbers()
{
    int i,j,p,q,cnt;
    for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            if(mines[i][j]!=-1)
            {
                cnt=0;
                for(p=i-1;p-i+1<3;++p)
                {
                    for(q=j-1;q-j+1<3;++q)
                    {
                        if(isValid(p,q))
                        {
                            if(mines[p][q] == -1) cnt++;
                        }
                    }
                }
                mines[i][j] = cnt;
            }
        }
    }
}

void GameBoard::unCover(int i, int j)
{
    if(COVERCOUNT == ROW*COL) //WHEN BOARD CLICKED FOR THE FIRST TIME
    {
        placeFLOWERs(i,j);
        placeNumbers();
    }

    if(mines[i][j]>0)
    {
        isCovered[i][j] = 0;
        COVERCOUNT--;
    }
    else if(mines[i][j] == -1)
    {
        int p;
        /*for(p=0;p<FLOWERCOUNT;p++)
        {
            isCovered[FLOWERVector[p].x][FLOWERVector[p].y] = 0;
            COVERCOUNT--;
        }*/
        int e,f;
        for(e=0;e<ROW;e++)
        {
            for(f=0;f<COL;f++)
            {
                isCovered[e][f] = 0;
            }
        }
        COVERCOUNT = 0;
        GAMEOVER = true;
    }
    else
    {
        queue< pair<int,int> > que;
        int p,q;
        que.push(pair<int,int> (i,j));
        isCovered[i][j] = 0;
        COVERCOUNT--;
        while(que.size())
        {
            p=que.front().first-1;
            q=que.front().second;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first-1;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first-1;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second-1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            p=que.front().first+1;
            q=que.front().second+1;
            if(isValid(p,q))
            {
                if(mines[p][q]!=-1 && isCovered[p][q])
                {
                    isCovered[p][q] = 0;
                    COVERCOUNT--;
                    if(mines[p][q] == 0)
                        que.push(pair<int,int> (p,q));
                }
            }
            que.pop();
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameBoard::printBoard()
{
    int i,j;
    cout<<'\t';
    for(i=0;i<COL;i++)
    {
        cout<<(i+1)/10<<' ';
    }
    cout<<"\n\t";
    for(i=0;i<COL;i++)
    {
        cout<<(i+1)%10<<' ';
    }
    cout<<"\n\n";
    for(i=0;i<ROW;i++)
    {
        cout<<(i+1)<<'\t';
        for(j=0;j<COL;j++)
        {
            if(isCovered[i][j])
            {
                cout<<char(127)<<' ';
            }
            else if(mines[i][j] == 0)
            {
                cout<<char(176)<<' ';
            }
            else if(mines[i][j] == -1)
            {
                cout<<char(219)<<' ';
            }
            else cout<<mines[i][j]<<' ';
        }
        cout<<'\n';
    }
}

void GameBoard::askUser()
{
    int i,j;
    cout<<"\nENTER ROW: ";
    do
    {
        cin>>i;
        if(i<=0 || i>ROW) cout<<"INVALID. RE-ENTER: ";
    }while(i<=0 || i>ROW);
    cout<<"ENTER COL: ";
    do
    {
        cin>>j;
        if(j<=0 || j>COL) cout<<"INVALID. RE-ENTER: ";
    }while(j<=0 || j>COL);
    unCover(i-1,j-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GameBoard::GameBoard()
{
    //Initialize the board with rows and columns. Initialize mines[][]
    setUpBoards();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void newGame(int row=10, int col=10, int dif=1)
{
    ROW = row;
    COL = col;
    int p;
    switch(dif)
    {
    case 1:
        p = 8;
        break;
    case 2:
        p = 6;
        break;
    default:
        p=4;
    }
    FLOWERCOUNT = ROW*COL/p;
    COVERCOUNT = ROW*COL;
    GAMEOVER = false;
    WIN = false;
    GameBoard g;
    g.printBoard();
    cout<<"\nCOVERED AREA: "<<COVERCOUNT<<"\n\n";
    while(!GAMEOVER && !WIN)
    {
        g.askUser();
        system("CLS");
        g.printBoard();
        cout<<"\nCOVERED AREA: "<<COVERCOUNT<<"\n\n";
        if(COVERCOUNT <= FLOWERCOUNT)
        {
            WIN = true;
        }
    }
    if(GAMEOVER)
    {
        cout<<"GAME OVER\n";
    }
    else if (WIN)
    {
        cout<<"YOU WON\n";
    }
}

void createMenu()
{
    cout<<"1)NEW GAME\n";
    cout<<"2)INSTRUCTIONS\n";
    cout<<"3)EXIT\n\nENTER CHOICE:";
}

void printInstructions()
{
    cout<<"THERE ARE 2 TYPES OF TILES:\n\na)COVERED\nb)UNCOVERED\n\n";
    cout<<"COVERED:\t\t\t"<<char(127)<<"\n-------\n";
    cout<<"UNCOVERED: \n---------\n\ti)EMPTY:\t\t"<<char(176)<<"\n\tii)NUMBERED:\t\t1,2,3,4,5,6,7,8\n\tiii)FLOWER:\t\t"<<char(219)<<"\n\n";
    cout<<"A ROWxCOL BOARD WILL BE PRESENTED, WHICH, INITIALLY HAS ALL TILES COVERED. YOUR TASK IS TO UNCOVER ALL TILES THAT ARE NOT HAVING FLOWERS.\n";
    cout<<"IF YOU SUCCEED TO DO SO, YOU WIN. IF YOU STEP ON A BOMB, YOU LOSE.\n\n";
    cout<<"YOU HAVE TO ENTER ROW AND COL OF A PARTICULAR TILE TO UNCOVER THAT TILE.\n";
    cout<<"EACH NUMBERED TILE REPRESENTS THE NUMBER OF FLOWERS ADJACENT TO THAT NUMBERED TILE.\n";
}
int main()
{
    int ch;
    do
    {
        system("CLS");
        createMenu();
        cin>>ch;
        system("CLS");
        switch(ch)
        {
            case 1:
                int dif;
                cout<<"SELECT DIFFICULTY:\n1)EASY\n2)MED\n3)HARD\n\nENTER CHOICE: ";
                do
                {
                    cin>>dif;
                    if(dif<1 || dif>3) cout<<"INVALID. RE-ENTER: ";
                }while(dif<1 || dif>3);
                system("CLS");
                int op;
                cout<<"SELECT GAME TYPE:\n1)DEFAULT\n2)CUSTOM\n\nENTER CHOICE: ";
                cin>>op;
                system("CLS");
                switch(op)
                {
                case 2:
                    int row,col;
                    cout<<"ENTER #ROWS(>0): ";
                    do
                    {
                        cin>>row;
                        if(row<=0) cout<<"INVALID. RE-ENTER #ROWS: ";
                    }while(row<=0);
                    cout<<"ENTER #COLUMNS(>0): ";
                    do
                    {
                        cin>>col;
                        if(col<=0) cout<<"INVALID. RE-ENTER #COLUMNS: ";
                    }while(col<=0);
                    //FLOWERcount = row*col/8;
                    //cout<<"#FLOWERS 12.5%ROW*COL= "<<FLOWERcount;
                    //getch();
                    system("CLS");
                    newGame(row,col,dif);
                    cout<<"\nPress ENTER to continue";
                    break;
                default:
                    newGame(10,10,dif);
                    cout<<"\nPress ENTER to continue";
                    break;
                }
                break;
            case 2:
                printInstructions();
                cout<<"\nPress ENTER to continue";
                break;
            default:
                cout<<"Press ENTER to continue";
        }
        getch();
    }while(ch!=3);
    return 0;
}

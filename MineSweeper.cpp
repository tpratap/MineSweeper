#include<iostream>
#include<cstdlib>
#include<vector>
#include<ctime>
#include<string>
#include<set>
#include<queue>
#include<utility>

using namespace std;

class MineSweeper {
    vector<vector<int>> mines;
    set<pair<int, int>> bombs;
    static vector<string> numList;
    static vector<pair<int, int>> dirs;
public:
    MineSweeper(int, int);
    int print(); //To Print mines board and return number of unrevealed
    bool check(int, int, bool); //To detonate bomb for once
    bool isValid(int, int); //To check valid row,col
    void bfs(int, int); //To traverse
    void startGame(); //MineSweeper game start
    void gameOver(bool); //Win or lose Screen
};

vector<string> MineSweeper::numList = {"01", "02", "03", "04", "05", "06", "07", "08", "09",
      "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "21", "22", 
      "23", "24", "25", "26", "27", "28", "29", "30"
};

vector<pair<int, int>> MineSweeper::dirs = {
    {0, -1}, {-1, 0}, {1, 0}, {0, 1}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}
};

MineSweeper::MineSweeper(int row, int col) {
    mines.resize(row);
    for(int i = 0; i < row; ++i) {
        mines[i].resize(col, -1);
    }
    int limit;
    switch(row) {
        case 9: limit = 10; break;
        case 16: limit = 40; break;
        case 30: limit = 99; break; 
    }
    vector<int> r,c;
    srand(1);
    for(int i = 0; i < limit; ++i) {
        int l = rand()%row;
        r.push_back(l);
    }
    srand(2);
    for(int i = 0; i < limit; ++i) {
        int l = rand()%col;
        c.push_back(l);
    }
    for(int i = 0; i < limit; ++i) {
        bombs.insert({r[i], c[i]});
    }
    print();
}

bool MineSweeper::check(int r, int c, bool del = false) {
    if(bombs.find(make_pair(r,c)) != bombs.end()) {
        if(del)
            bombs.erase({r,c});
        return true;
    }
    return false;
}

bool MineSweeper::isValid(int r, int c) {
    if(r < 0 || r >= mines.size() || c >= mines[0].size() || c < 0)
        return false;
    return true;
}

int MineSweeper::print() {
    bool first_row = true;
    int col = 0;
    int unrevealed = 0;
    cout<<"   ";
    for(vector<int> row: mines) {
        if(first_row) {
            for(int i = 0; i < mines[0].size(); ++i) {
                cout<<numList[i]<<" ";
            }
            cout<<"\n";
            first_row = false;
        }
        cout<<numList[col++]<<" ";
        for(int val: row) {
            //Uncovered
            if(val == -1) {
                unrevealed++;
                cout<<"??";
            } else if(val == 0) {
                cout<<"  ";
            } else {
                cout<<numList[val-1];
            }
            cout<<" ";
        }
        cout<<"\n";
    }
    cout<<"Press Enter to continue ... \n";
    char m = cin.get();
    // m = cin.get();
    return unrevealed;
}

void MineSweeper::bfs(int r, int c) {
    queue<pair<int, int>> q;
    q.push({r, c});
    while(!q.empty()) {
        int i = q.front().first;
        int j = q.front().second;
        q.pop();
        if(mines[i][j] != -1) {
            continue;
        }
        mines[i][j] = 0;
        int cnt = 0;
        for(int k = 0; k < dirs.size(); ++k) {
            int x = i + dirs[k].first;
            int y = j + dirs[k].second;
            if(isValid(x, y) && check(x, y)) {
                cnt++;
            }
        }
        if(cnt > 0) {
            mines[i][j] = cnt;
        } else {
            for(int k = 0; k < dirs.size(); ++k) {
                int x = i + dirs[k].first;
                int y = j + dirs[k].second;
                if(isValid(x, y)) {
                    q.push({x,y});
                }
            }        
        }
    }
}

void MineSweeper::startGame() {
    int r, c;
    bool wins = false, lost = false;
    do {
        system("clear");    
        int unrevealed = print();
        cout<<"Enter row col to unreveal that position: \n";
        do {
            cin>>r>>c;
            if(!isValid(r-1, c-1)) {
                cout<<"Invalid position, Re-Enter Valid One: \n";
            }
        } while(!isValid(r-1, c-1) && mines[r-1][c-1] != -1);
        if(check(r-1, c-1)) {
            lost = true;
        } else if(unrevealed == bombs.size()) {
            wins = true;
        } else {
            bfs(r-1, c-1);
        }
    } while (!wins && !lost);
    if(wins)
        gameOver(true);
    else
        gameOver(false);
}

void MineSweeper::gameOver(bool wins) {
    system("clear");
    if(wins) {
        cout<<"Congratulations, you won this game... \n";
    } else {
        cout<<"You lost, better luck next time... \n";
    }
    cout<<"\n ** means presense of bomb at that point. \n\n";
    bool first_row = true;
    int col = 0;
    cout<<"   ";
    for(int i = 0; i < mines.size(); ++i) {
        if(first_row) {
            for(int i = 0; i < mines[0].size(); ++i) {
                cout<<numList[i]<<" ";
            }
            cout<<"\n";
            first_row = false;
        }
        cout<<numList[col++]<<" ";
        for(int j = 0; j < mines[i].size(); ++j) {
            int val = mines[i][j];
            if(check(i,j)) {
                cout<<"**";
            }
            else if(val == -1) {
                cout<<"??";
            } else if(val == 0) {
                cout<<"  ";
            } else {
                cout<<numList[val-1];
            }
            cout<<" ";
        }
        cout<<"\n";
    }
    cout<<"Press Enter to continue ... \n";
    char m = cin.get();
    // m = cin.get();
}

void newGame() {
    int diff;
    cout<<"Select Difficulty Level: \n";
    cout<<"\t 1. Easy (9 * 9) \n";
    cout<<"\t 2. Medium (16 * 16) \n";
    cout<<"\t 3. Hard (30 * 16) \n";
    do {
        cin>>diff;
        if(diff < 1 || diff > 3)
            cout<<"Invalid Difficulty Level, Re-Enter Valid One: \n";
    } while(diff < 1 || diff > 3);
    int row,col;
    switch(diff) {
        case 1: row = 9; col = 9; break;
        case 2: row = 16; col = 16; break;
        case 3: row = 30; col = 16; break; 
    }    
    MineSweeper M(row, col);
    system("clear");
    int ch, r, c;
    cout<<"Select Game Type: \n";
    cout<<"\t 1. Default (Random) \n";
    cout<<"\t 2. Custom \n";
    cout<<"\t ENTER CHOICE: \n";
    cin>>ch;
    if(ch == 2) {
        system("clear");
        cout<<"Enter row col to unreveal that position: \n";
        do {
            cin>>r>>c;
            if(!M.isValid(r-1, c-1)) {
                cout<<"Invalid position, Re-Enter Valid One: \n";
            }
        } while(!M.isValid(r-1, c-1));
    } else {
        srand(time(0));
        r = rand()%row + 1;
        c = rand()%col + 1;
    }
    M.check(r-1, c-1, true);
    M.bfs(r-1, c-1);
    M.print();
    M.startGame();
}

void printInstructions() {
   cout<<"There are two types of Tiles: \n";
   cout<<"\t 1. Covered:   \n";
   cout<<"\t 2. Uncovered \n";
   cout<<"\t Furthur uncovered tiles can be of three types: \n";
   cout<<"\t\t a. Numbered: (1,2,3,4,5,6,7,8) that denotes number of bombs in neighbours. \n";
   cout<<"\t\t b. Empty: . that denotes there is no bombs in neighbours \n";
   cout<<"\t\t c. Bombs: * that denotes detonated bombs in neighbours \n";
   cout<<"Task is to uncover the covered tiles with the help of hints: \n";
}

void createMENU() {
    int ch;
    do {
        /*********************START MENU**********************/
        system("clear");
        cout<<"1)NEW GAME\n";
        cout<<"2)INSTRUCTIONS\n";
        cout<<"3)EXIT\n\nENTER CHOICE:";
        cin>>ch;

        /*********************DIFFICULTY MENU**********************/
        system("clear");
        switch(ch) {
            case 1: newGame(); break;
            case 2: printInstructions(); break;
        }

        //For waiting until Enter is clicked
        int c = cin.get();
        c = cin.get();

    } while(ch != 3);
}

int main() {
    createMENU();
    return 0;
}

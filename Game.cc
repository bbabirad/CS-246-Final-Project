class Game {

    Board board;
    vector<unique_ptr<Player>> players;
    int currentPlayerIndex = 0;
    unique_ptr<Display> display1;
    unique_ptr<Display> display2;
    bool finished = false;
    int numPlayers = 2;

    Game(int nplayers=2);

    Player& currentPlayer();
    Player& otherPlayer();

    void nextTurn();

    void display();
	
    bool executeMove(char linkId, const string &dirStr);

    /*
    bool useAbility(int idx, const vector<string>& args) {
        Player &p = currentPlayer();
        if (idx < 1 || idx > (int)p.abilities.size()) { cout << "Invalid ability id\n"; return false; }
        Ability* a = p.abilities[idx-1].get();
        if (a->hasUsed()) { cout << "Ability already used\n"; return false; }
        bool ok = a->use(*this, args);
        if (ok) { a->markUsed(); cout << "Ability used: " << a->name() << "\n"; }
        else cout << "Ability failed or canceled\n";
        return ok;
    }
*/
    Player* getPlayerById(int id);

    void checkWinConditions();
};

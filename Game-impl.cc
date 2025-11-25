import <iostream>;
import <string>;

import Game;
import Player;
/*
CHANGES: 
CREATED IMPL FILE FOR GAME
SHOULD *** be apart of command interface
REMOVED ALL ABILITY REFERENCES
CHANGED IN BOUNDS PARAMETERS



*/
Game(int nplayers=2) : numPlayers(nplayers) {
        for (int i=0;i<numPlayers;i++) players.emplace_back(make_unique<Player>(i+1));
        display1 = make_unique<struct TextDisplay>();
        display2 = make_unique<struct TextDisplay>();
    }

Player& currentPlayer() { return *players[currentPlayerIndex]; }
Player& otherPlayer() { return *players[(currentPlayerIndex+1)%numPlayers]; }

void nextTurn() { currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers; }

void display() {
        display->render(board, &players[0]);
    }

bool executeMove(char linkId, const string &dirStr) {
        Player &p = currentPlayer();
        Link* link = p.getLinkByIdChar(linkId);
        if (!link) { cout << "Invalid link id\n"; return false; }
        
        if (!board.findLinkPos(link, from)) { cout << "Link not on board\n"; return false; }

        // parse direction into delta ***
        int dr=0, dc=0;
        if (dirStr == "up") {
	       	dr=-1;
		dc=0; 
	} else if (dirStr == "down") { 
		dr=1;
		dc=0; 
	}
        else if (dirStr == "left") { 
		dr=0; 
		dc=-1; 
	}
        else if (dirStr == "right") { 
		dr=0; 
		dc=1; 
	}
        else { 
		cout << "Invalid direction\n"; 
		return false; 
	}
	link->row += dr;
	link->column += dc;


	bool offEdge = !board.inBounds(link->row, link->column);

        if (offEdge) {
            // allowed only if moving off opponent starting edge
            int pid = p.playerID;
            // moving off top (r < 0) allowed if opponent started at top (player 1 started top)
            // Interpret: moving off opponent's starting edge (the edge where opponent placed their links originally)
            if (pid == 1 && link->row >= Board::R) {
                // player1 moving off bottom (opponent start)
            } else if (pid == 2 && link->row < 0) {
                // player2 moving off top
            } else {
                cout << "Cannot move off that edge\n"; return false;
            }
            // handle download (moving off)
            board.clearPos(from);
            p.addDownloaded(link);
            p.removeLink(link);
            cout << p.getName() << " downloaded their own link by moving off opponent edge.\n";
            return true;
        }

//WALK ME THROUGH THIS
	Link* at = board.get(to);
        if (!at) {
            board.clearPos(from);
            board.place(link, to);
            cout << "Moved.\n";
            return true;
        } else {
            if (at->getOwner() == &p) { cout << "Cannot move onto your own link.\n"; return false; }
            bool initiatorWins = link->battle(*at, true);
            Player *winner = initiatorWins ? &p : at->getOwner();
            Player *loser = initiatorWins ? at->getOwner() : &p;
            Link *loserLink = initiatorWins ? at : link;
            Link *winnerLink = initiatorWins ? link : at;

            board.clearPos(from); board.clearPos(to);
            board.place(winnerLink, to);

            winner->addDownloaded(loserLink);
            loser->removeLink(loserLink);
            loser->addAgainstMe(loserLink);
            cout << winner->getName() << " wins battle and downloads opponent link.\n";
            return true;
        }
    }

Player* getPlayerById(int id) {
        for (auto &pl : players) if (pl->playerID == id) return pl.get();
        return nullptr;
    }

void checkWinConditions() {
        for (auto &pl : players) {
            if (pl->downloadsData >= 4) { cout << pl->getName() << " wins by downloading 4 data!\n"; finished = true; }
            else if (pl->downloadsVirus >= 4) { cout << pl->getName() << " loses (downloaded 4 viruses)!\n"; finished = true; }
}

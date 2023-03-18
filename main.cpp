#include <iostream>
#include <fstream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void process_file(connection *C, const string &filename, const string &mode)
{
    ifstream file(filename);
    if (file.fail()){
        cerr << "Cannot open the txt file: " << filename << endl;
        return;
    }

    string line;
    stringstream ss;

    if (mode == "table") {
        string sql_commands;
        while (getline(file, line)){
            sql_commands += line;
        }
        work W(*C);
        W.exec(sql_commands);
        W.commit();
    } else {
        while (getline(file, line)){
            ss.clear();
            ss << line;

            if (mode == "state") {
                int state_id;
                string name;
                ss >> state_id >> name;
                add_state(C, name);
            } else if (mode == "color") {
                int color_id;
                string name;
                ss >> color_id >> name;
                add_color(C, name);
            } else if (mode == "team") {
                int team_id, state_id, color_id, wins, losses;
                string name;
                ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
                add_team(C, name, state_id, color_id, wins, losses);
            } else if (mode == "player") {
                int player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
                double spg, bpg;
                string first_name, last_name;
                ss >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
                add_player(C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
            }
        }
    }

    file.close();
}

int main (int argc, char *argv[]) 
{
    connection *C;

    try {
        C = new connection("dbname=ACC_BBALL user=postgres password=hhwohen6");
        if (C->is_open()) {
            cout << "Opened database successfully: " << C->dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }

    process_file(C, "table.sql", "table");
    process_file(C, "state.txt", "state");
    process_file(C, "color.txt", "color");
    process_file(C, "team.txt", "team");
    process_file(C, "player.txt", "player");

    exercise(C);

    C->disconnect();

    return 0;
}

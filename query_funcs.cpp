#include "query_funcs.h"
#include "assert.h"
#include <iomanip> // std::setprecision()

// W.quote() : -> string that can be put into SQL: Eg 'O'Brien -> 'O''Brien

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    W.exec("INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES(" +
           to_string(team_id) + "," + to_string(jersey_num) + "," + W.quote(first_name) + "," + W.quote(last_name) + "," +
           to_string(mpg) + "," + to_string(ppg) + "," + to_string(rpg) + "," + to_string(apg) + "," + to_string(spg) + "," + to_string(bpg) + ");");
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    W.exec("INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES(" +
           W.quote(name) + "," + to_string(state_id) + "," + to_string(color_id) + "," + to_string(wins) + "," + to_string(losses) + ");");
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    W.exec("INSERT INTO STATE (NAME) VALUES(" + W.quote(name) + ");");
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    W.exec("INSERT INTO COLOR (NAME) VALUES(" + W.quote(name) + ");");
    W.commit();
}


/**
 * @param exist to confirm if there is already a SQL command
 * ss and exist need "&", global variable
*/

void use_pg(stringstream & ss, int use_pg, double min_pg, double max_pg, bool & exist, string mode)
{
    // MPG BETWEEN min_MPG AND max_MPG OR MPG IS NULL
    if (use_pg){
        if (exist == false){
            ss << " WHERE ";
        }
        else{
            ss << " AND ";
        }
        ss << "(" << mode << " BETWEEN " << min_pg << " AND " << max_pg << ")";
        exist = true;
    }
}


void query1(connection *C,
            int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    assert(use_mpg || use_ppg || use_rpg || use_apg || use_spg || use_bpg);
    stringstream ss;
    ss << "SELECT * FROM PLAYER";
    bool exist = false;

    use_pg(ss, use_mpg, min_mpg, max_mpg, exist, "MPG");
    use_pg(ss, use_ppg, min_ppg, max_ppg, exist, "PPG");
    use_pg(ss, use_rpg, min_rpg, max_rpg, exist, "RPG");
    use_pg(ss, use_apg, min_apg, max_apg, exist, "APG");
    use_pg(ss, use_spg, min_spg, max_spg, exist, "SPG");
    use_pg(ss, use_bpg, min_bpg, max_bpg, exist, "BPG");

    ss << ";";

    // Create a work object
    work W(*C);
    // Execute the query and store the result
    result R(W.exec(ss.str()));

    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;

    for (const auto &it : R)
    {
        cout << it[0] << " " << it[1] << " " << it[2] << " "
             << it[3] << " " << it[4] << " " << it[5] << " "
             << it[6] << " " << it[7] << " " << it[8] << " ";

        cout << fixed << setprecision(1) << it[9].as<double>() << " " << it[10].as<double>() << endl;
    }
}


void query2(connection *C, string team_color)
{
    work W(*C);
    result R(W.exec("SELECT TEAM.NAME FROM TEAM, COLOR WHERE COLOR.COLOR_ID = TEAM.COLOR_ID AND COLOR.NAME = " + W.quote(team_color) + ";"));
    cout << "TEAM" << endl;
    for (const auto &it : R)
    {
        cout << it[0] << endl;
    }
}



void query3(connection *C, string team_name)
{
    work W(*C);
    result R(W.exec("SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = " + W.quote(team_name) + " ORDER BY PPG DESC;"));
    cout << "FIRST_NAME LAST_NAME" << endl;
    for (const auto &it : R)
    {
        cout << it[0] << " " << it[1] << endl;
    }
}



void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    result R(W.exec("SELECT PLAYER.UNIFORM_NUM, PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, STATE, COLOR, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND TEAM.STATE_ID = STATE.STATE_ID AND STATE.NAME = " + W.quote(team_state) + " AND COLOR.NAME = " + W.quote(team_color) + ";"));
    cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;
    for (const auto &it : R)
    {
        cout << it[0] << " " << it[1] << " " << it[2] << endl;
    }
}



void query5(connection *C, int num_wins)
{
    work W(*C);
    result R(W.exec("SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE TEAM.TEAM_ID = PLAYER.TEAM_ID AND TEAM.WINS > " + to_string(num_wins) + ";"));
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
    for (const auto &it : R)
    {
        cout << it[0] << " " << it[1] << " " << it[2] << " " << it[3] << endl;
    }
}

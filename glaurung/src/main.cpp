/*
  Glaurung, a UCI chess playing engine.
  Copyright (C) 2004-2008 Tord Romstad

  Glaurung is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  Glaurung is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


////
//// Includes
////

#include <cstdlib>
#include <iostream>

#include "bitboard.h"
#include "direction.h"
#include "endgame.h"
#include "evaluate.h"
#include "material.h"
#include "mersenne.h"
#include "misc.h"
#include "movepick.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "uci.h"
#include "ucioption.h"


//// 
//// Functions
////

int main(int argc, char *argv[]) {

  // Disable IO buffering
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  std::cout.rdbuf()->pubsetbuf(NULL, 0);
  std::cin.rdbuf()->pubsetbuf(NULL, 0);

  // Initialization

  init_mersenne();
  init_direction_table();
  init_bitboards();
  init_uci_options();
  Position::init_zobrist();
  Position::init_piece_square_tables();
  MaterialInfo::init();
  MovePicker::init_phase_table();
  init_eval(1);
  init_bitbases();
  init_threads();


  // Print copyright notice
  std::cout << engine_name() << ".  "
            << "Copyright (C) 2004-2008 Tord Romstad."
            << std::endl;

  // Enter UCI mode
  uci_main_loop();

  return 0;
}

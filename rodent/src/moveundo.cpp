/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2019 Pawel Koziol
Copyright (C) 2020-2020 Bernhard C. Maerz

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "rodent.h"

void POS::UndoMove(int move, UNDO *u) {

    eColor sd = ~mSide;
    eColor op = ~sd;
    int fsq = Fsq(move);
    int tsq = Tsq(move);
    int ftp = Tp(mPc[tsq]); // moving piece
    int ttp = u->mTtpUd;

    mCFlags   = u->mCFlagsUd;
    mEpSq     = u->mEpSqUd;
    mRevMoves = u->mRevMovesUd;
    mHashKey  = u->mHashKeyUd;
    mPawnKey  = u->mPawnKeyUd;

    mHead--;

    mPc[tsq] = NO_PC;
    mPc[fsq] = Pc(sd, ftp);
    mClBb[sd] ^= SqBb(tsq);
    mClBb[sd] |= SqBb(fsq);
    mTpBb[ftp] ^= SqBb(fsq) ^ SqBb(tsq);

    // Change king location

    if (ftp == K) mKingSq[sd] = fsq;

    // Uncapture enemy piece

    if (ttp != NO_TP && MoveType(move) != CASTLE) {
        mPc[tsq] = Pc(op, ttp);
        mClBb[op] ^= SqBb(tsq);
        mTpBb[ttp] ^= SqBb(tsq);
        mMgScore[op] += Par.mg_mat[ttp];
        mEgScore[op] += Par.eg_mat[ttp];
        mPhase += ph_value[ttp];
        mCnt[op][ttp]++;
    }

    switch (MoveType(move)) {

        case NORMAL:
            break;

        case CASTLE:

            // define complementary rook move

            switch (tsq) {
                case C1: { fsq = Castle_W_RQ; tsq = D1; break; }
                case G1: { fsq = Castle_W_RK; tsq = F1; break; }
                case C8: { fsq = Castle_B_RQ; tsq = D8; break; }
                case G8: { fsq = Castle_B_RK; tsq = F8; break; }
            }

            if (mPc[tsq] != Pc(sd, K)) {
                mPc[tsq] = NO_PC;
                mClBb[sd] ^= SqBb(tsq);
            }
            mPc[fsq] = Pc(sd, R);
            mClBb[sd] ^= SqBb(fsq);
            mTpBb[R] ^= SqBb(fsq) ^ SqBb(tsq);
            break;

        case EP_CAP:
            tsq ^= 8;
            mPc[tsq] = Pc(op, P);
            mClBb[op] ^= SqBb(tsq);
            mTpBb[P] ^= SqBb(tsq);
            mMgScore[op] += Par.mg_mat[P];
            mEgScore[op] += Par.eg_mat[P];
            mPhase += ph_value[P];
            mCnt[op][P]++;
            break;

        case EP_SET:
            break;

        case N_PROM: case B_PROM: case R_PROM: case Q_PROM:
            mPc[fsq] = Pc(sd, P);
            mTpBb[P] ^= SqBb(fsq);
            mTpBb[ftp] ^= SqBb(fsq);
            mMgScore[sd] += Par.mg_mat[P] - Par.mg_mat[ftp];
            mEgScore[sd] += Par.eg_mat[P] - Par.eg_mat[ftp];
            mPhase += ph_value[P] - ph_value[ftp];
            mCnt[sd][P]++;
            mCnt[sd][ftp]--;
            break;
    }

    mSide = ~mSide;
}

void POS::UndoNull(UNDO *u) {

    mEpSq    = u->mEpSqUd;
    mHashKey = u->mHashKeyUd;
    mHead--;
    mRevMoves--;
    mSide = ~mSide;
}

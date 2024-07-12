//-------------------------------------------------------
//
// File: EvaluationTypes.h
// Author: Edward Koch
// Description: Holds the Definitions of Evaluations types
//
// Revision History
// Author    Date    Description
//-------------------------------------------------------
// E. Koch    04/10/23    Initial Creation of Evaluation Types
//-------------------------------------------------------
#pragma once

// Piece Scores
const int PAWN_SCORE = 100;
const int KNIGHT_SCORE = 305; // 1 Knight is better than 3 Pawns
const int BISHOP_SCORE = 325;
const int ROOK_SCORE = 530; // 1 Rook is better than a Minor Piece and 2 Pawns
const int QUEEN_SCORE = 900;
const int KING_SCORE = 20000;

const int PROTECTION_SCORE = 1; // Give points for protecting more pieces
const int MOBILITY_SCORE = 2; // Give points for being able to move

const int CHECK_SCORE = 50; // Being in check is Rough

const int WIN_SCORE = 25000; // Incentivise Winning
const int LOSE_SCORE = -25000; // Deincentivise Losing
const int DRAW_SCORE = -200; // Slightly Deincentivise drawing

// Index the piece scores by type of piece
const int PieceScores[NUM_PIECES] =
{
    PAWN_SCORE,
    KNIGHT_SCORE,
    BISHOP_SCORE,
    ROOK_SCORE,
    QUEEN_SCORE,
    KING_SCORE
};
// Modify the scores, positive for white, negative for black
const int ScoreModifier[BLACK + 1] = { 1, -1 };

// All of the Piece Square Tables for each piece
#include "PieceSquareTables.inc"

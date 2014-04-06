/*
 * transition_table.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef TRANSITION_TABLE_H_
#define TRANSITION_TABLE_H_

// Transition table for player
struct transition_table : mpl::vector<
	//    Start                     Event            Next                        Action        Guard
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < Init                    , evInitGame     , WaitForAvailColors        , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < WaitForAvailColors      , evInitColors   , WaitForClientColors       , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < WaitForClientColors     , evClientColors , SelectColorMode           , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < SelectColorMode         , evInitGame     , GmMoveDone                , delSsms     , none            >,
	Row < SelectColorMode         , evMove         , none                      , Defer       , none            >,
	Row < SelectColorMode         , evDice         , none                      , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveDone              , evMove         , GmMoveMeeple              , /*TODO meepleMoved*/none, none>,
	Row < GmMoveDone              , evDice         , GmDice                    , popDiceData , gMyColor        >,
	Row < GmMoveDone              , evDice         , GmWaitForShowDice         , popDiceData , gNotMyColor     >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmDice                  , evEnter        , GmWaitForShowDice         , sendDiceDone, none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmWaitForShowDice       , evWaitForShowDiceAgain, GmWaitForShowDice  , none        , none            >,
	Row < GmWaitForShowDice       , evShowDice     , GmShowDice                , showDice    , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmShowDice              , evEnter        , GmCheckMovedMeeple        , none        , gMoveAllowed    >,
	Row < GmShowDice              , evMove         , GmMoveMeeple              , none        , none            >,
	Row < GmShowDice              , none           , GmMoveDone                , waitSomeTime, gMoveNotAllowed >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveMeeple            , evMoveDone     , GmMoveDone                , none        , none            >,
	Row < GmMoveMeeple            , evCheckDst     , GmCheckDestination        , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmCheckMovedMeeple      , evMeepleOK     , GmSendMovedMeeple         , sendMovedMeeple, none         >,
	Row < GmCheckMovedMeeple      , evMeepleNotOK  , GmShowDice                , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmSendMovedMeeple       , evMove         , GmCheckDestination        , none        , none            >,
	Row < GmSendMovedMeeple       , evMeepleNotOK  , GmShowDice				   , resetMove   , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmCheckDestination      , evMeepleNotOK  , GmSearchForMeeple         , transferFromTo, none          >,
	Row < GmCheckDestination      , evMoveDone     , GmMoveDone                , meepleMoved , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmSearchForMeeple       , evMeepleOK     , GmFoundMeeple             , transferFromToIllegal, none   >,
	Row < GmSearchForMeeple       , evMeepleNotOK  , GmMoveMeeplesByHand       , transferFromTo, none          >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmFoundMeeple           , evMoveDone     , GmMoveDone                , meepleMoved , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveMeeplesByHand     , evEnter        , GmReconfigureMeepleIDs    , transferFromTo, none          >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmReconfigureMeepleIDs  , evMeepleNotOK  , GmMoveMeeplesByHand       , none        , none            >,
	Row < GmReconfigureMeepleIDs  , evMeepleOK     , GmMoveMeeplesToCorrectPos , transferFromToAll, none       >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveMeeplesToCorrectPos,evMoveDone     , GmMoveDone                , meepleMoved , none            >
> {};

#endif /* TRANSITION_TABLE_H_ */

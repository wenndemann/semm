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
	//    Start                     Event         Next                        Action        Guard
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
	Row < GmMoveDone              , evDice         , GmDice                    , checkDice   , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmDice                  , evEnter        , GmWaitForShowDice         , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmWaitForShowDice       , evShowDice     , GmShowDice                , showDice    , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmShowDice              , evEnter        , GmCheckMovedMeeple        , none        , gMoveAllowed    >,
	Row < GmShowDice              , evMove         , GmMoveMeeple              , none        , none            >,
	Row < GmShowDice              , none           , GmMoveDone                , waitSomeTime, gMoveNotAllowed >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveMeeple            , evMoveDone     , GmMoveDone                , none        , none            >,
	Row < GmMoveMeeple            , evMove         , none                      , Defer       , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmCheckMovedMeeple      , evMeepleOK     , GmSendMovedMeeple         , sendMovedMeeple, none         >,
	Row < GmCheckMovedMeeple      , evMeepleNotOK  , GmShowDice                , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmSendMovedMeeple       , evMove         , GmCheckDestination        , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmCheckDestination      , evMeepleNotOK  , GmSearchForMeeple         , none        , none            >,
	Row < GmCheckDestination      , evMoveDone     , GmMoveDone                , meepleMoved , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmSearchForMeeple       , evMeepleOK     , GmFoundMeeple             , meepleMoved , none            >,
	Row < GmSearchForMeeple       , evMeepleNotOK  , GmMoveMeeplesByHand       , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmMoveMeeplesByHand     , evEnter        , GmReconfigureMeepleIDs    , none        , none            >,
	//  +-------------------------+----------------+---------------------------+-------------+------------------+
	Row < GmReconfigureMeepleIDs  , evMoveDone     , GmMoveDone                , none        , none            >
> {};

#endif /* TRANSITION_TABLE_H_ */

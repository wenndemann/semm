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
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < Init                    , evInitGame     , WaitForAvailColors        , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < WaitForAvailColors      , evInitColors   , WaitForClientColors       , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < WaitForClientColors     , evClientColors , SelectColorMode           , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < SelectColorMode         , evInitGame     , GmMoveDone                , delSsms     , none      >,
//	Row < SelectColorMode         , evMove         , none                      , Defer       , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmMoveDone              , evMove         , GmMoveMeeple              , none        , none      >,
	Row < GmMoveDone              , evDice         , GmDice                    , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmDice                  , evEnter        , GmWaitForShowDie          , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmWaitForShowDie        , evShowDie      , GmShowDie                 , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmShowDie               , evEnter        , GmCheckMovedMeeple        , none        , none      >,
	Row < GmShowDie               , evMove         , GmMoveMeeple              , none        , none      >,
	Row < GmShowDie               , evMoveDone     , GmMoveDone                , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmMoveMeeple            , evMoveDone     , GmMoveDone                , none        , none      >,
//	Row < GmMoveMeeple            , evMove         , none                      , Defer       , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmCheckMovedMeeple      , evMeepleOK     , GmSendMovedMeeple         , none        , none      >,
	Row < GmCheckMovedMeeple      , evMeepleNotOK  , GmShowDie                 , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmSendMovedMeeple       , evMove         , GmCheckDestination        , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmCheckDestination      , evMeepleNotOK  , GmSearchForMeeple         , none        , none      >,
	Row < GmCheckDestination      , evMoveDone     , GmMoveDone                , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmSearchForMeeple       , evMeepleOK     , GmFoundMeeple             , none        , none      >,
	Row < GmSearchForMeeple       , evMeepleNotOK  , GmMoveMeeplesByHand       , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
//	Row < GmMoveMeeplesByHand     , evEnter        , GmReconfigureMeepleIDs    , none        , none      >,
	//  +-------------------------+----------------+---------------------------+-------------+------------+
	Row < GmReconfigureMeepleIDs  , evMoveDone     , GmMoveDone                , none        , none      >
> {};

#endif /* TRANSITION_TABLE_H_ */

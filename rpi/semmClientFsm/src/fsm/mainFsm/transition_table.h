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

	//    Start                     Event         Next                      Action        Guard
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < Init                    , evInitGame     , WaitForAvailColors      , none        , none      >,
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < WaitForAvailColors      , evInitColors   , WaitForClientColors     , none        , none      >,
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < WaitForClientColors     , evClientColors , SelectColorMode         , none        , none      >,
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < SelectColorMode         , evInitGame     , GmMoveDone              , delSsms     , none      >,
	Row < SelectColorMode         , evMove         , none                    , Defer       , none      >,
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < GmMoveDone              , evMove         , GmMoveMeeple            , none        , none      >,
	//  +-------------------------+--------------+-------------------------+-------------+-------------+
	Row < GmMoveMeeple            , evMoveDone     , GmMoveDone              , none        , none      >,
	Row < GmMoveMeeple            , evMove         , none                    , Defer       , none      >

	/*
	//    Start     Event         Next      Action                      Guard
	//  +---------+-------------+---------+----------------------------+----------------------+
	Row < Stopped , play        , Playing , ActionSequence_
											 <mpl::vector<
											 TestFct,start_playback> > , DummyGuard           >,
	Row < Stopped , open_close  , Open    , open_drawer                , none                 >,
	Row < Stopped , stop        , Stopped , none                       , none                 >,
	//  +---------+-------------+---------+----------------------------+----------------------+
	Row < Open    , open_close  , Empty   , close_drawer               , none                 >,
	//  +---------+-------------+---------+----------------------------+----------------------+
	Row < Empty   , open_close  , Open    , open_drawer                , none                 >,
	Row < Empty   , cd_detected , Stopped , store_cd_info              , And_<good_disk_format,
																			  always_true>    >,
	//  +---------+-------------+---------+----------------------------+----------------------+
	Row < Playing , stop        , Stopped , stop_playback              , none                 >,
	Row < Playing , paus        , Paused  , pause_playback             , none                 >,
	Row < Playing , open_close  , Open    , stop_and_open              , none                 >,
	//  +---------+-------------+---------+----------------------------+----------------------+
	Row < Paused  , end_pause   , Playing , resume_playback            , none                 >,
	Row < Paused  , stop        , Stopped , stop_playback              , none                 >,
	Row < Paused  , open_close  , Open    , stop_and_open              , none                 >
	//  +---------+-------------+---------+----------------------------+----------------------+
	 */
> {};

#endif /* TRANSITION_TABLE_H_ */

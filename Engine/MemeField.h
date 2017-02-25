#pragma once

#include "Vei2.h"

class MemeField
{
	class Tile
	{
		enum class State
		{
			HIDDEN,
			FLAGGED,
			REVEALED
		};

		State state;
		bool hasMeme;

	public:
		void SpawnMeme();

		bool HasMeme() const;
		bool IsRevealed() const;
		bool IsFlagged() const;

		void Reveal();
		void Flag();
		void Unflag();
	};

	static constexpr int GRID_WIDTH = 20;
	static constexpr int GRID_HEIGHT = 16;

	Tile field[ GRID_WIDTH * GRID_HEIGHT ];

	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;

public:
	MemeField( int nMemes );
};

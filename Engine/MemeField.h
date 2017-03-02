#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "RectI.h"

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

		State state = State::HIDDEN;
		bool hasMeme = false;
		int nNeighbourMemes = -1;

	public:
		void SpawnMeme();

		bool HasMeme() const;
		bool IsRevealed() const;
		bool IsFlagged() const;

		void Reveal();
		void ToggleFlagged();
		void SetNeighbourCount( int neighbourMemeCount );

		void Draw( const Vei2& screenPos,bool fucked,Graphics& gfx ) const;
	};

	static constexpr int GRID_WIDTH = 20;
	static constexpr int GRID_HEIGHT = 16;

	Tile field[ GRID_WIDTH * GRID_HEIGHT ];

	bool fucked = false;

	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;

	int CountNeighbourMemes( const Vei2& gridPos ) const;

public:
	MemeField( int nMemes );

	RectI GetRect() const;
	bool IsInsideField( const Vei2& gridPos ) const;

	void OnLeftClick( const Vei2& mousePos );
	void OnRightClick( const Vei2& mousePos );

	void Draw( Graphics& gfx ) const;
};

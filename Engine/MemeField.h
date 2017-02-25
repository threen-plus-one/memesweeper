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

	public:
		void SpawnMeme();

		bool HasMeme() const;
		bool IsRevealed() const;
		bool IsFlagged() const;

		void Reveal();
		void Flag();
		void Unflag();

		void Draw( const Vei2& screenPos,Graphics& gfx ) const;
	};

	static constexpr int GRID_WIDTH = 20;
	static constexpr int GRID_HEIGHT = 16;

	Tile field[ GRID_WIDTH * GRID_HEIGHT ];

	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;

public:
	MemeField( int nMemes );

	RectI GetRect() const;
	bool IsInsideField( const Vei2& gridPos ) const;

	void OnLeftClick( const Vei2& mousePos );

	void Draw( Graphics& gfx ) const;
};

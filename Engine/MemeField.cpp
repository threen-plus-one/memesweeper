#include "MemeField.h"

#include "SpriteCodex.h"
#include <cassert>
#include <random>
#include <algorithm>

void MemeField::Tile::SpawnMeme()
{
	assert( !hasMeme );
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::REVEALED;
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::FLAGGED;
}

void MemeField::Tile::Reveal()
{
	assert( state == State::HIDDEN );
	state = State::REVEALED;
}

void MemeField::Tile::ToggleFlagged()
{
	assert( state != State::REVEALED );

	if( state == State::HIDDEN )
	{
		state = State::FLAGGED;
	}
	else
	{
		state = State::HIDDEN;
	}
}

void MemeField::Tile::SetNeighbourCount( int neighbourMemeCount )
{
	assert( nNeighbourMemes == -1 );
	assert( neighbourMemeCount >= 0 && neighbourMemeCount <= 8 );
	nNeighbourMemes = neighbourMemeCount;
}

void MemeField::Tile::Draw( const Vei2& screenPos,bool fucked,Graphics& gfx ) const
{
	if( !fucked )
	{
		switch( state )
		{
		case State::HIDDEN:
			SpriteCodex::DrawTileButton( screenPos,gfx );
			break;
		case State::FLAGGED:
			SpriteCodex::DrawTileButton( screenPos,gfx );
			SpriteCodex::DrawTileFlag( screenPos,gfx );
			break;
		case State::REVEALED:
			if( hasMeme )
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileNumber( screenPos,nNeighbourMemes,gfx );
			}
			break;
		}
	}
	else
	{
		switch( state )
		{
		case State::HIDDEN:
			if( hasMeme )
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileButton( screenPos,gfx );
			}
			break;
		case State::FLAGGED:
			if( hasMeme )
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
				SpriteCodex::DrawTileFlag( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
				SpriteCodex::DrawTileCross( screenPos,gfx );
			}
			break;
		case State::REVEALED:
			if( hasMeme )
			{
				SpriteCodex::DrawTileBombRed( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileNumber( screenPos,nNeighbourMemes,gfx );
			}
			break;
		}
	}
}

MemeField::Tile& MemeField::TileAt( const Vei2& gridPos )
{
	assert( IsInsideField( gridPos ) );
	return field[ GRID_WIDTH * gridPos.y + gridPos.x ];
}

const MemeField::Tile& MemeField::TileAt( const Vei2& gridPos ) const
{
	assert( IsInsideField( gridPos ) );
	return field[ GRID_WIDTH * gridPos.y + gridPos.x ];
}

int MemeField::CountNeighbourMemes( const Vei2& gridPos ) const
{
	assert( IsInsideField( gridPos ) );
	const int xStart = std::max( 0,gridPos.x - 1 );
	const int xEnd = std::min( GRID_WIDTH - 1,gridPos.x + 1 );
	const int yStart = std::max( 0,gridPos.y - 1 );
	const int yEnd = std::min( GRID_HEIGHT - 1,gridPos.y + 1 );

	int neighbourCount = 0;
	for( Vei2 loopVec = { xStart,yStart }; loopVec.y <= yEnd; ++loopVec.y )
	{
		for( loopVec.x = xStart; loopVec.x <= xEnd; ++loopVec.x )
		{
			if( TileAt( loopVec ).HasMeme() )
			{
				++neighbourCount;
			}
		}
	}

	if( TileAt( gridPos ).HasMeme() )
	{
		--neighbourCount;
	}

	return neighbourCount;
}

MemeField::MemeField( int nMemes )
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> xpos( 0,GRID_WIDTH - 1 );
	std::uniform_int_distribution<int> ypos( 0,GRID_HEIGHT - 1 );

	for( ; nMemes > 0; --nMemes )
	{
		Vei2 gridPos;
		do
		{
			gridPos = { xpos( rng ),ypos( rng ) };
		}
		while( TileAt( gridPos ).HasMeme() );
		TileAt( gridPos ).SpawnMeme();
	}

	for( Vei2 loopVec = { 0,0 }; loopVec.y < GRID_HEIGHT; ++loopVec.y )
	{
		for( loopVec.x = 0; loopVec.x < GRID_WIDTH; ++loopVec.x )
		{
			TileAt( loopVec ).SetNeighbourCount( CountNeighbourMemes( loopVec ) );
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI( Vei2( 0,0 ),Vei2( GRID_WIDTH,GRID_HEIGHT ) * SpriteCodex::tileSize );
}

bool MemeField::IsInsideField( const Vei2& gridPos ) const
{
	return
		gridPos.x >= 0 &&
		gridPos.x < GRID_WIDTH &&
		gridPos.y >= 0 &&
		gridPos.y < GRID_HEIGHT;
}

void MemeField::OnLeftClick( const Vei2& mousePos )
{
	if( !fucked )
	{
		const Vei2 gridPos = mousePos / SpriteCodex::tileSize;
		if( IsInsideField( gridPos ) && !TileAt( gridPos ).IsRevealed() && !TileAt( gridPos ).IsFlagged() )
		{
			TileAt( gridPos ).Reveal();
			if( TileAt( gridPos ).HasMeme() )
			{
				fucked = true;
			}
		}
	}
}

void MemeField::OnRightClick( const Vei2& mousePos )
{
	if( !fucked )
	{
		const Vei2 gridPos = mousePos / SpriteCodex::tileSize;
		if( IsInsideField( gridPos ) && !TileAt( gridPos ).IsRevealed() )
		{
			TileAt( gridPos ).ToggleFlagged();
		}
	}
}

void MemeField::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( GetRect(),SpriteCodex::baseColor );

	for( Vei2 gridPos = { 0,0 }; gridPos.y < GRID_HEIGHT; ++gridPos.y )
	{
		for( gridPos.x = 0; gridPos.x < GRID_WIDTH; ++gridPos.x )
		{
			TileAt( gridPos ).Draw( gridPos * SpriteCodex::tileSize,fucked,gfx );
		}
	}
}

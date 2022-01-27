#include "Headers/MemeField.h"
#include <assert.h>
#include <random>
#include "../Engine/SpriteCodex.h"

void MemeField::Tile::SpawnMeme ( )
{
    assert ( !hasMeme );
    hasMeme = true;
}

bool MemeField::Tile::HasMeme ( ) const
{
    return hasMeme;
}

void MemeField::Tile::Draw ( const Vei2 screenPos, Graphics& gfx ) const
{
    switch( state )
    {
        case MemeField::Tile::State::Hidden:
            SpriteCodex::DrawTileButton;
            break;
        case MemeField::Tile::State::Flagged:
            SpriteCodex::DrawTileButton;
            SpriteCodex::DrawTileFlag;
            break;
        case MemeField::Tile::State::Revealed:
            if( !hasMeme )
            {
                SpriteCodex::DrawTile0;
            }
            else
            {
                SpriteCodex::DrawTileBomb;
            }
            break;
    }
}

void MemeField::Tile::Reveal ( )
{
    assert ( state == State::Hidden );
    state = State::Revealed;
}

bool MemeField::Tile::IsRevealed ( ) const
{
    return state == State::Revealed;
}

MemeField::MemeField ( int numberOfMemes )
{
    assert ( numberOfMemes > 0 && numberOfMemes < width* height );
    std::random_device rd;
    std::mt19937 rng ( rd ( ) );
    std::uniform_int_distribution<int> xDist ( 0, width - 1 );
    std::uniform_int_distribution<int> yDist ( 0, height - 1 );
    for( int i = 0; i < numberOfMemes; i++ )
    {
        Vei2 spawnPos;
        do
        {
            //Calling constructor here
            spawnPos = { xDist ( rng ),yDist ( rng ) };
        }
        while( TileAt ( spawnPos ).HasMeme ( ) );
        TileAt(spawnPos).SpawnMeme ( );
    }
}

void MemeField::Draw ( Graphics& gfx )
{
    for( Vei2 gridPos = {0,0}; gridPos.y < width; gridPos.y++ )
    {
        for(gridPos.x = 0 ; gridPos.x < height; gridPos.x++ )
        {
            TileAt ( gridPos ).Draw ( gridPos * SpriteCodex::tileSize, gfx );
        }
    }
}

MemeField::Tile& MemeField::TileAt ( const Vei2& gridPos )
{
    return field[ gridPos.y * width + gridPos.x ];
}

const MemeField::Tile& MemeField::TileAt ( const Vei2& gridPos ) const
{
    return field[ gridPos.y * width + gridPos.x ];
}

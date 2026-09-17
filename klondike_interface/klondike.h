#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <string>
#include <vector>


// ============================================================
// CARD
// ============================================================

enum class Suit : uint8_t
{
    CLUB,
    DIAMOND,
    HEART,
    SPADE
};

class Card
{
public:

    Card() = default;

    Card(uint8_t value, Suit suit);

    uint8_t value() const;
    Suit suit() const;

    bool isBelow(const Card& card) const;
    bool isOppositeSuit(const Card& card) const;
    bool canAttach(const Card& card) const;

    std::string name() const;
    std::string title() const;

private:

    uint8_t m_value = 0;
    Suit m_suit = Suit::CLUB;
};


// ============================================================
// DECK
// ============================================================

class Deck
{
public:

    Deck();

    void reset();
    void shuffle();

    bool empty() const;

    Card flipCard();

    std::vector<Card> dealCards(size_t count);

    size_t size() const;

private:

    std::vector<Card> m_cards;

    std::mt19937 m_rng;
};


// ============================================================
// FOUNDATION
// ============================================================

class Foundation
{
public:

    Foundation();

    bool addCard(const Card& card);

    const Card* getTopCard(Suit suit) const;

    bool gameWon() const;

    size_t size(Suit suit) const;

    const std::vector<Card>& getPile(Suit suit) const;

private:

    std::array<std::vector<Card>, 4> m_stacks;

    static size_t suitIndex(Suit suit);
};


// ============================================================
// STOCK / WASTE
// ============================================================

class StockWaste
{
public:

    StockWaste() = default;

    explicit StockWaste(std::vector<Card> cards);

    bool stockToWaste();

    Card* getWaste();

    const Card* getWaste() const;

    Card popWasteCard();

    size_t stockSize() const;
    size_t wasteSize() const;

    bool stockEmpty() const;
    bool wasteEmpty() const;

    const std::vector<Card>& stock() const;
    const std::vector<Card>& waste() const;

private:

    std::vector<Card> m_stock;
    std::vector<Card> m_waste;
};


// ============================================================
// TABLEAU
// ============================================================

class Tableau
{
public:

    Tableau() = default;

    explicit Tableau(const std::array<std::vector<Card>, 7>& columns);

    bool flipCard(size_t column);

    bool addCards(
        const std::vector<Card>& cards,
        size_t column
    );

    bool tableauToTableau(
        size_t source,
        size_t destination
    );

    bool tableauToFoundation(
        Foundation& foundation,
        size_t column
    );

    bool wasteToTableau(
        StockWaste& stockWaste,
        size_t column
    );

    size_t pileLength() const;

    size_t hiddenSize(size_t column) const;
    size_t shownSize(size_t column) const;

    const std::vector<Card>& hidden(size_t column) const;
    const std::vector<Card>& shown(size_t column) const;

    std::vector<Card>& hidden(size_t column);
    std::vector<Card>& shown(size_t column);

private:

    std::array<std::vector<Card>, 7> m_hidden;
    std::array<std::vector<Card>, 7> m_shown;
};


// ============================================================
// COMPLETE GAME
// ============================================================

class klondike_game
{
public:

    klondike_game();

    void newGame();

    // --------------------------------------------------------
    // Actions
    // --------------------------------------------------------

    bool stockToWaste();

    bool wasteToFoundation();

    bool wasteToTableau(size_t column);

    bool tableauToFoundation(size_t column);

    bool tableauToTableau(
        size_t source,
        size_t destination
    );

    // --------------------------------------------------------
    // State
    // --------------------------------------------------------

    bool gameWon() const;

    const Tableau& tableau() const;
    const Foundation& foundation() const;
    const StockWaste& stockWaste() const;

private:

    Deck m_deck;
    Tableau m_tableau;
    Foundation m_foundation;
    StockWaste m_stockWaste;
};
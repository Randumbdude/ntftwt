#include "klondike.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>


// ============================================================
// CARD
// ============================================================

Card::Card(uint8_t value, Suit suit)
    : m_value(value),
    m_suit(suit)
{
}

uint8_t Card::value() const
{
    return m_value;
}

Suit Card::suit() const
{
    return m_suit;
}


bool Card::isBelow(const Card& card) const
{
    return m_value == card.m_value - 1;
}


bool Card::isOppositeSuit(const Card& card) const
{
    bool thisBlack =
        m_suit == Suit::CLUB ||
        m_suit == Suit::SPADE;

    bool otherBlack =
        card.m_suit == Suit::CLUB ||
        card.m_suit == Suit::SPADE;

    return thisBlack != otherBlack;
}


bool Card::canAttach(const Card& card) const
{
    return card.isBelow(*this) &&
        card.isOppositeSuit(*this);
}


std::string Card::name() const
{
    switch (m_value)
    {
    case 1:
        return "A";

    case 2:
        return "2";

    case 3:
        return "3";

    case 4:
        return "4";

    case 5:
        return "5";

    case 6:
        return "6";

    case 7:
        return "7";

    case 8:
        return "8";

    case 9:
        return "9";

    case 10:
        return "10";

    case 11:
        return "J";

    case 12:
        return "Q";

    case 13:
        return "K";

    default:
        return "?";
    }
}


std::string Card::title() const
{
    switch (m_suit)
    {
    case Suit::CLUB:
        return name() + "club";

    case Suit::DIAMOND:
        return name() + "diam";

    case Suit::HEART:
        return name() + "heart";

    case Suit::SPADE:
        return name() + "spade";
    }

    return "?";
}


// ============================================================
// DECK
// ============================================================

Deck::Deck()
    : m_rng(std::random_device{}())
{
    reset();
}


void Deck::reset()
{
    m_cards.clear();
    m_cards.reserve(52);

    for (uint8_t value = 1; value <= 13; ++value)
    {
        m_cards.emplace_back(value, Suit::CLUB);
        m_cards.emplace_back(value, Suit::DIAMOND);
        m_cards.emplace_back(value, Suit::HEART);
        m_cards.emplace_back(value, Suit::SPADE);
    }
}


void Deck::shuffle()
{
    std::shuffle(
        m_cards.begin(),
        m_cards.end(),
        m_rng
    );
}


bool Deck::empty() const
{
    return m_cards.empty();
}


Card Deck::flipCard()
{
    if (m_cards.empty())
        throw std::runtime_error("Deck is empty.");

    Card card = m_cards.back();

    m_cards.pop_back();

    return card;
}


std::vector<Card> Deck::dealCards(size_t count)
{
    if (count > m_cards.size())
        throw std::runtime_error("Not enough cards in deck.");

    std::vector<Card> result;

    result.reserve(count);

    for (size_t i = 0; i < count; ++i)
        result.push_back(flipCard());

    return result;
}


size_t Deck::size() const
{
    return m_cards.size();
}


// ============================================================
// FOUNDATION
// ============================================================

size_t Foundation::suitIndex(Suit suit)
{
    switch (suit)
    {
    case Suit::CLUB:
        return 0;

    case Suit::DIAMOND:
        return 1;

    case Suit::HEART:
        return 2;

    case Suit::SPADE:
        return 3;
    }

    return 0;
}


Foundation::Foundation()
{
    for (auto& pile : m_stacks)
        pile.clear();
}


bool Foundation::addCard(const Card& card)
{
    auto& pile = m_stacks[suitIndex(card.suit())];

    // Empty foundation requires Ace.
    if (pile.empty())
    {
        if (card.value() != 1)
            return false;

        pile.push_back(card);

        return true;
    }

    // Otherwise card must be exactly one higher.
    if (pile.back().value() + 1 != card.value())
        return false;

    pile.push_back(card);

    return true;
}


const Card* Foundation::getTopCard(Suit suit) const
{
    const auto& pile = m_stacks[suitIndex(suit)];

    if (pile.empty())
        return nullptr;

    return &pile.back();
}


bool Foundation::gameWon() const
{
    for (const auto& pile : m_stacks)
    {
        if (pile.size() != 13)
            return false;
    }

    return true;
}


size_t Foundation::size(Suit suit) const
{
    return m_stacks[suitIndex(suit)].size();
}


const std::vector<Card>& Foundation::getPile(Suit suit) const
{
    return m_stacks[suitIndex(suit)];
}


// ============================================================
// STOCK / WASTE
// ============================================================

StockWaste::StockWaste(std::vector<Card> cards)
    : m_stock(std::move(cards))
{
}


bool StockWaste::stockToWaste()
{
    if (m_stock.empty())
    {
        if (m_waste.empty())
            return false;

        // Python:
        //
        // self.waste.reverse()
        // self.deck = self.waste.copy()
        // self.waste.clear()

        std::reverse(
            m_waste.begin(),
            m_waste.end()
        );

        m_stock = std::move(m_waste);

        m_waste.clear();
    }

    m_waste.push_back(m_stock.back());

    m_stock.pop_back();

    return true;
}


Card* StockWaste::getWaste()
{
    if (m_waste.empty())
        return nullptr;

    return &m_waste.back();
}


const Card* StockWaste::getWaste() const
{
    if (m_waste.empty())
        return nullptr;

    return &m_waste.back();
}


Card StockWaste::popWasteCard()
{
    if (m_waste.empty())
        throw std::runtime_error("Waste is empty.");

    Card card = m_waste.back();

    m_waste.pop_back();

    return card;
}


size_t StockWaste::stockSize() const
{
    return m_stock.size();
}


size_t StockWaste::wasteSize() const
{
    return m_waste.size();
}


bool StockWaste::stockEmpty() const
{
    return m_stock.empty();
}


bool StockWaste::wasteEmpty() const
{
    return m_waste.empty();
}


const std::vector<Card>& StockWaste::stock() const
{
    return m_stock;
}


const std::vector<Card>& StockWaste::waste() const
{
    return m_waste;
}


// ============================================================
// TABLEAU
// ============================================================

Tableau::Tableau(
    const std::array<std::vector<Card>, 7>& columns
)
{
    for (size_t i = 0; i < 7; ++i)
    {
        m_hidden[i] = columns[i];

        if (!m_hidden[i].empty())
        {
            m_shown[i].push_back(
                m_hidden[i].back()
            );

            m_hidden[i].pop_back();
        }
    }
}


bool Tableau::flipCard(size_t column)
{
    if (column >= 7)
        return false;

    if (m_hidden[column].empty())
        return false;

    m_shown[column].push_back(
        m_hidden[column].back()
    );

    m_hidden[column].pop_back();

    return true;
}


bool Tableau::addCards(
    const std::vector<Card>& cards,
    size_t column
)
{
    if (column >= 7 || cards.empty())
        return false;

    auto& destination = m_shown[column];

    // Empty tableau column requires King.
    if (destination.empty())
    {
        if (cards.front().value() != 13)
            return false;

        destination.insert(
            destination.end(),
            cards.begin(),
            cards.end()
        );

        return true;
    }

    // Otherwise first card must attach to the
    // current bottom card.
    if (!destination.back().canAttach(cards.front()))
        return false;

    destination.insert(
        destination.end(),
        cards.begin(),
        cards.end()
    );

    return true;
}


bool Tableau::tableauToTableau(
    size_t source,
    size_t destination
)
{
    if (source >= 7 || destination >= 7)
        return false;

    if (source == destination)
        return false;

    auto& sourceCards = m_shown[source];

    if (sourceCards.empty())
        return false;

    // Try every possible starting card.
    for (size_t index = 0;
        index < sourceCards.size();
        ++index)
    {
        std::vector<Card> moving(
            sourceCards.begin() + index,
            sourceCards.end()
        );

        if (addCards(moving, destination))
        {
            sourceCards.erase(
                sourceCards.begin() + index,
                sourceCards.end()
            );

            // If we exposed a hidden card,
            // flip it.
            if (index == 0)
                flipCard(source);

            return true;
        }
    }

    return false;
}


bool Tableau::tableauToFoundation(
    Foundation& foundation,
    size_t column
)
{
    if (column >= 7)
        return false;

    auto& cards = m_shown[column];

    if (cards.empty())
        return false;

    Card card = cards.back();

    if (!foundation.addCard(card))
        return false;

    cards.pop_back();

    // If the shown pile became empty,
    // expose a hidden card.
    if (cards.empty())
        flipCard(column);

    return true;
}


bool Tableau::wasteToTableau(
    StockWaste& stockWaste,
    size_t column
)
{
    if (column >= 7)
        return false;

    Card* waste = stockWaste.getWaste();

    if (waste == nullptr)
        return false;

    std::vector<Card> cards;
    cards.push_back(*waste);

    if (!addCards(cards, column))
        return false;

    stockWaste.popWasteCard();

    return true;
}


size_t Tableau::pileLength() const
{
    size_t longest = 0;

    for (size_t i = 0; i < 7; ++i)
    {
        size_t length =
            m_hidden[i].size() +
            m_shown[i].size();

        longest = std::max(
            longest,
            length
        );
    }

    return longest;
}


size_t Tableau::hiddenSize(size_t column) const
{
    return m_hidden[column].size();
}


size_t Tableau::shownSize(size_t column) const
{
    return m_shown[column].size();
}


const std::vector<Card>& Tableau::hidden(size_t column) const
{
    return m_hidden[column];
}


const std::vector<Card>& Tableau::shown(size_t column) const
{
    return m_shown[column];
}


std::vector<Card>& Tableau::hidden(size_t column)
{
    return m_hidden[column];
}


std::vector<Card>& Tableau::shown(size_t column)
{
    return m_shown[column];
}


// ============================================================
// COMPLETE GAME
// ============================================================

klondike_game::klondike_game()
{
    newGame();
}


void klondike_game::newGame()
{
    m_deck.reset();
    m_deck.shuffle();

    std::array<std::vector<Card>, 7> columns;

    // Python:
    //
    // [d.deal_cards(x) for x in range(1,8)]
    //
    // produces:
    //
    // 1 card
    // 2 cards
    // 3 cards
    // ...
    // 7 cards

    for (size_t column = 0;
        column < 7;
        ++column)
    {
        columns[column] =
            m_deck.dealCards(column + 1);
    }

    m_tableau = Tableau(columns);

    // Remaining 24 cards go to stock.
    m_stockWaste =
        StockWaste(m_deck.dealCards(
            m_deck.size()
        ));

    m_foundation = Foundation();
}


bool klondike_game::stockToWaste()
{
    return m_stockWaste.stockToWaste();
}


bool klondike_game::wasteToFoundation()
{
    const Card* card =
        m_stockWaste.getWaste();

    if (card == nullptr)
        return false;

    if (!m_foundation.addCard(*card))
        return false;

    m_stockWaste.popWasteCard();

    return true;
}


bool klondike_game::wasteToTableau(size_t column)
{
    return m_tableau.wasteToTableau(
        m_stockWaste,
        column
    );
}


bool klondike_game::tableauToFoundation(size_t column)
{
    return m_tableau.tableauToFoundation(
        m_foundation,
        column
    );
}


bool klondike_game::tableauToTableau(
    size_t source,
    size_t destination
)
{
    return m_tableau.tableauToTableau(
        source,
        destination
    );
}


bool klondike_game::gameWon() const
{
    return m_foundation.gameWon();
}


const Tableau& klondike_game::tableau() const
{
    return m_tableau;
}


const Foundation& klondike_game::foundation() const
{
    return m_foundation;
}


const StockWaste& klondike_game::stockWaste() const
{
    return m_stockWaste;
}
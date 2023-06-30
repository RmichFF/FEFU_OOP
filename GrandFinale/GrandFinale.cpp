#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <algorithm>

std::string crimes[5] = { "inchoate crimes", "crimes against public order", "crimes against property", "violent crimes", "political crimes" };
char security[3] = { 'A','S','O' };

struct Prisoner
{
    enum class crimeType { InchoateCrime = 0, PublicOrderCrime, PropertyCrime, ViolentCrime, PoliticalCrime };
    int serialNumber = 0;
    unsigned int age = 0;
    bool lifeSentence = false;
    crimeType sentence = crimeType::PoliticalCrime;
    Prisoner() {};
    Prisoner(int serialNumber, unsigned int age, bool lifeSentence, crimeType sentence)
    {
        if (serialNumber >= 10000 && serialNumber < 100000)
            this->serialNumber = serialNumber;
        else throw std::invalid_argument("invalid");

        if (age >= 18 && age <= 90)
            this->age = age;
        else throw std::invalid_argument("invalid");

        this->lifeSentence = lifeSentence;
        this->sentence = sentence;
    };
    
    friend bool operator==(const Prisoner& a, const Prisoner& b);
    friend bool operator!=(const Prisoner& a, const Prisoner& b);
    friend bool operator>(const Prisoner& a, const Prisoner& b);
    friend bool operator<(const Prisoner& a, const Prisoner& b);
};

bool operator==(const Prisoner& a, const Prisoner& b)
{
    return (a.lifeSentence == b.lifeSentence && a.sentence == b.sentence);
}

bool operator!=(const Prisoner& a, const Prisoner& b)
{
    return !(a == b);
}

bool operator>(const Prisoner& a, const Prisoner& b)
{
    return ((a.lifeSentence && !b.lifeSentence) || (a.lifeSentence == b.lifeSentence && a.sentence > b.sentence));
}

bool operator<(const Prisoner& a, const Prisoner& b)
{
    return ((!a.lifeSentence && b.lifeSentence) || (a.lifeSentence == b.lifeSentence && a.sentence < b.sentence));
}


template <typename Type>
class Container
{
public:
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;

        Iterator(Container::Iterator& o) : m_ptr(o.m_ptr) {}
        Iterator(Container::Iterator&& o) noexcept : m_ptr(std::move(o.m_ptr))
        {
            o.m_ptr = nullptr;
        }
        Iterator(pointer ptr) : m_ptr(ptr) {}
        ~Iterator() {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator& operator--() { m_ptr--; return *this; }
        Iterator& operator+(int n) { m_ptr = m_ptr + n; return *this; }
        Iterator& operator-(int n) { m_ptr = m_ptr - n; return *this; }
        Iterator& operator+=(int n) { m_ptr = m_ptr + n; return *this; }
        Iterator& operator-=(int n) { m_ptr = m_ptr - n; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
        friend bool operator< (const Iterator& a, const Iterator& b) { return a.m_ptr < b.m_ptr; };
        friend bool operator> (const Iterator& a, const Iterator& b) { return a.m_ptr > b.m_ptr; };
        friend bool operator>= (const Iterator& a, const Iterator& b) { return (a.m_ptr > b.m_ptr) || (a.m_ptr == b.m_ptr); };
        friend bool operator<= (const Iterator& a, const Iterator& b) { return (a.m_ptr < b.m_ptr) || (a.m_ptr == b.m_ptr); };

        Iterator& operator=(const Container::Iterator& other)
        {
            if (this == &other)
                return *this;
            *m_ptr = *other;
            return *this;
        }

        Iterator& operator=(Iterator&& other) noexcept
        {
            if (this == &other)
                return *this;
            m_ptr = std::move(other.m_ptr);
            other.m_ptr = nullptr;
            return *this;
        }

        void swap(Iterator& other) { std::swap(*m_ptr, *other); }

    private:
        pointer m_ptr;
    };
    Container(int S) 
    { 
        m_data = new Type[S];
        current = 0; 
    }
    ~Container()
    {
        delete[] m_data;
    }
    Iterator begin() { return Iterator(&m_data[0]); }
    Iterator end() { return Iterator(&m_data[current]); }
    int GetOccupancy() { return current; }
    void Add(Type& val)
    {
        m_data[current] = val;
        current++;
    }

private:
    Type* m_data;
    int current;
};


class NewsChannel
{
public:
    NewsChannel(std::string name)
    {
        this->name = name;
    }
    void Update(Prisoner prisoner)
    {
        std::cout << std::format("{} reporting! No{} is now arrested for ", name, prisoner.serialNumber) + crimes[(int)prisoner.sentence] << std::endl;
    }
    friend bool operator==(const NewsChannel& a, const NewsChannel& b);
private:
    std::string name;
};

bool operator==(const NewsChannel& a, const NewsChannel& b)
{
    return (a.name == b.name);
}


class Prison
{
protected:
    std::string name = "";
    std::string location = "";

private:
    Container<Prisoner>* prisonerContainer = nullptr;
    int capacity = 0;
    std::vector<NewsChannel> News;

public:
    Prison() {};
    Prison(std::string name, std::string location, int capacity) 
    {
        this->name = name;
        this->location = location;
        if (capacity >= 50)
            this->capacity = capacity;
        else throw std::invalid_argument("invalid");
        this->prisonerContainer = new Container<Prisoner>(capacity);
    };

    virtual void ShowInfo()
    {
        std::cout << std::format("{}, located in {}.\nStated capacity: {:^7}   Occupancy: {:>6.5}%\n",name, location, capacity, (float)(prisonerContainer->GetOccupancy())/capacity*100);
    }

    virtual void AddPrisoner(Prisoner& prisoner)
    {
        prisonerContainer->Add(prisoner);
        NotifyAllChannels(prisoner);
    }

    void ShowPrisoners(Prisoner::crimeType crime)
    {
        Container<Prisoner>::Iterator beg = prisonerContainer->begin();
        Container<Prisoner>::Iterator end = prisonerContainer->end();
        while (beg != end)
        {
            if (crime == beg->sentence)
            {
                std::cout << std::format("\nPrisoner No:{}; Age: {}\n", beg->serialNumber, beg->age);
                std::cout << (beg->lifeSentence ? "Life sentenced for " : "Sentenced for ") + crimes[static_cast<int>(beg->sentence)] << std::endl;
            }
            beg++;
        }
    }

    void AddNewsChannel(NewsChannel& ch)
    {
        News.push_back(ch);
    }

    void RemoveNewsChannel(NewsChannel ch)
    {
       News.erase(std::find(News.begin(), News.end(), ch));
    }

    void NotifyAllChannels(Prisoner prisoner)
    {
        for (NewsChannel i : News)
            i.Update(prisoner);
    }
};

class MSColony : Prison
{
public:
    MSColony(std::string name, std::string location, int capacity) : Prison(name, location, capacity) {};

    void ShowInfo()
    {
        std::cout << ("Maximum-Security colony ");
        Prison::ShowInfo();
    }

    void AddPrisoner(Prisoner& prisoner)
    {
        if ((int)prisoner.sentence == 4 || prisoner.lifeSentence)
            Prison::AddPrisoner(prisoner);
    }

    void ShowPrisoners(Prisoner::crimeType crime)
    {
        Prison::ShowPrisoners(crime);
    }
};

class MSPrison : Prison
{
protected:
    char securityClass;
public:
    MSPrison(int sClass, std::string name, std::string location, int capacity) : Prison(name, location, capacity)
    {
        this->securityClass = security[sClass];
    }

    void ShowInfo()
    {
        std::cout << std::format("Class {} ", securityClass);
        Prison::ShowInfo();
    }

    void AddPrisoner(Prisoner& prisoner)
    {
        if ((int)prisoner.sentence > 2)
            Prison::AddPrisoner(prisoner);
    }

    void ShowPrisoners(Prisoner::crimeType crime)
    {
        Prison::ShowPrisoners(crime);
    }

    void AddNewsChannel(NewsChannel ch)
    {
        Prison::AddNewsChannel(ch);
    }

    void RemoveNewsChannel(NewsChannel ch)
    {
        Prison::RemoveNewsChannel(ch);
    }

    void NotifyAllChannels(Prisoner prisoner)
    {
        Prison::NotifyAllChannels(prisoner);
    }
};

class AbstractPrisonFactory
{
public:
    virtual Prison* createPrison(std::string name, std::string location, int capacity) = 0;
};

class ColonyFactory : public AbstractPrisonFactory
{
public:
    Prison* createPrison(std::string name, std::string location, int capacity)
    {
        return new Prison(name, location, capacity);
    }
};

int main()
{
    Prison* W = new Prison("Black Bolphin", "Russia", 100);

    for (int i = 0; i < 30; i++)
    {
        Prisoner* p = new Prisoner(10000 + i, 40, false, (Prisoner::crimeType)(i % 5));
        W->AddPrisoner(*p);
    }
    W->ShowPrisoners(Prisoner::crimeType::PoliticalCrime);
    W->ShowInfo();

    std::cout << '\n';
    NewsChannel* C = new NewsChannel("pekaTV");
    W->AddNewsChannel(*C);

    std::cout << '\n';
    Prisoner* k = new Prisoner(12454, 44, false, Prisoner::crimeType::PublicOrderCrime);
    W->AddPrisoner(*k);
    std::cout << '\n';
    W->ShowInfo();

    std::cout << '\n';
    ColonyFactory* Fact = new ColonyFactory();
    Prison* E = Fact->createPrison("Guantanamo", "Cuba", 50);
    E->ShowInfo();
}


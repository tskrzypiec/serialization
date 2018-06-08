/*
 * Serializacja klasy
 *
 * Napisz swoj¹ implementacjê serializacji klasy:
 * - serializacja do pliku tekstowego lub binarnego w zale¿noœci od parametru programu
 * 		./nazwa --help
 * 		./nazwa txt
 * 		./nazwa bin
 * - klasa powinna sygnalizowaæ sytuacje wyj¹tkowe
 *
 */

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/version.hpp>

#define SERIALIZED_FNAME "serialized"

enum class FuelType { PETROL, DIESEL };
enum class TireType { ALLSEASON, WINTER, OFFROAD };
enum class Colour { BLACK, RED, BLUE, GREEN };
enum class EmissionsStandards { EURO1, EURO2, EURO3, EURO4, EURO5 };
struct Wheels
{
    std::string manufacturer;
    TireType tire_type;
    int size_inches;

    bool operator==(const Wheels& rhs) const
    {
    	return std::tie(manufacturer, tire_type, size_inches)
    	== std::tie(rhs.manufacturer, rhs.tire_type, rhs.size_inches);
    };

    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & manufacturer;
		ar & tire_type;
		ar & size_inches;
	}
};

class Car {
public:
	Car() {};
	Car(FuelType fuel, std::array<Wheels, 4> wh, uint32_t speed, Colour col)
	{
		fuel_type = fuel;
		wheels = wh;
		max_speed = speed;
		colour = col;
	};
	FuelType getFuelType() { return fuel_type; };
	std::array<Wheels, 4> getWheels() { return wheels; };
	uint32_t getMaxSpeed() { return max_speed; };
	Colour getColour() { return colour; };
protected:
    FuelType fuel_type;
    std::array<Wheels, 4> wheels;
    uint32_t max_speed;
    Colour colour;
private:
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & fuel_type;
		ar & wheels;
		ar & max_speed;
		ar & colour;
	}
};

class PersonalCar: public Car {
public:
	PersonalCar() {};
	PersonalCar(uint8_t places, EmissionsStandards em, FuelType fuel, std::array<Wheels, 4> wh, uint32_t speed, Colour col) : Car(fuel, wh, speed, col)
	{
		number_of_places = places;
		emission = em;
	};
	uint8_t getNumberOfPlaces() { return number_of_places; };
	EmissionsStandards getEmission() { return emission; };

	bool operator==(const PersonalCar& rhs) const
	{
	    return std::tie(number_of_places, emission, fuel_type, wheels, max_speed, colour)
	    == std::tie(rhs.number_of_places, rhs.emission, rhs.fuel_type, rhs.wheels, rhs.max_speed, rhs.colour);
	};
protected:
    uint8_t number_of_places;
    EmissionsStandards emission;
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // serialize base class information
        ar & boost::serialization::base_object<Car>(*this);
        ar & number_of_places;
        ar & emission;
    }
};

BOOST_CLASS_VERSION(PersonalCar, 1)

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option>\n"
              << "Options:\n"
              << "\t--help\tshow this help message\n"
              << "\ttxt\tserialize to text file\n"
			  << "\tbin\tserialize to binary"
              << std::endl;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		show_usage(argv[0]);
		return 1;
	}

	// create instance of the class and serialize it
	PersonalCar pc(1, EmissionsStandards::EURO4, FuelType::PETROL, { "Goodyear", TireType::ALLSEASON, 26 }, 185, Colour::RED);
	std::ofstream ofs(SERIALIZED_FNAME);

	const std::string program_param = argv[1];
	if("txt" == program_param)
	{
		// save data to archive - txt
		std::cout << "txt" << std::endl;
		boost::archive::text_oarchive oa(ofs);
		oa << pc;
		ofs.close();

		// test: restore the class instance to its original state
		PersonalCar newpc;
		{
			std::ifstream ifs(SERIALIZED_FNAME);
			boost::archive::text_iarchive ia(ifs);
			// read class state from archive
			ia >> newpc;
			ifs.close();
			assert(pc==newpc);
		}

	}
    else if("bin" == program_param)
    {
    	// save data to archive - bin
    	std::cout << "bin" << std::endl;
    	boost::archive::binary_oarchive oa(ofs);
    	oa << pc;
    	ofs.close();

    	// test: restore the class instance to its original state
    	PersonalCar newpc;
    	{
    		std::ifstream ifs(SERIALIZED_FNAME);
    		boost::archive::binary_iarchive ia(ifs);
    		// read class state from archive
    		ia >> newpc;
    		ifs.close();
    		assert(pc==newpc);
    	}
    }
    else
    {
		show_usage(argv[0]);
		return 1;
    }

	return 0;
}

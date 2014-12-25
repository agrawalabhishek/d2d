/*
 * Copyright (c) 2014 K. Kumar (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <limits>
#include <map>
#include <string>

#include <boost/array.hpp>

#include <rapidjson/document.h>

#include <libsgp4/Eci.h>

#include <Astro/astro.hpp>

namespace d2d
{

//! 3-Vector.
typedef boost::array< double, 3 > Vector3;

//! 6-Vector.
typedef boost::array< double, 6 > Vector6;

//! State history.
typedef std::map< double, Vector6 > StateHistory;

//! JSON config iterator.
typedef rapidjson::Value::ConstMemberIterator ConfigIterator;

//! Sample Kepler orbit.
/*!
 * Samples a Kepler orbit and generates a state-history stored in a STL map (key=epoch). The
 * Kepler orbit is sampled by using propagate_lagrangian() provided with PyKep (Izzo, 2012).
 *
 * @param[in]  initialState           Initial Cartesian state [km; km/s]
 * @param[in]  propagationTime        Total propagation time [s]
 * @param[in]  numberOfSamples        Number of samples, distributed evenly over propagation time
 * @param[in]  gravitationalParameter Gravitational parameter of central body [km^3 s^-2]
 * @param[in]  initialEpoch           Epoch corresponding to initial Cartesian state
 *                                    (default = 0.0) [Julian date]
 * @return                            State-history of sampled Kepler orbit
 */
StateHistory sampleKeplerOrbit( const Vector6& initialState,
                                const double propagationTime,
                                const int numberOfSamples,
                                const double gravitationalParameter,
                                const double initialEpoch = 0.0 );

//! Convert SGP4 ECI object to state vector.
/*!
 * Converts a Cartesian state stored in an object of the Eci class in the SGP4 library () into a
 * boost::array of 6 elements.
 *
 * @sa Eci, boost::array
 * @param  state State stored in Eci object
 * @return       State stored in boost::array
 */
inline Vector6 getStateVector( const Eci state )
{
    Vector6 result;
    result[ astro::xPositionIndex ] = state.Position( ).x;
    result[ astro::yPositionIndex ] = state.Position( ).y;
    result[ astro::zPositionIndex ] = state.Position( ).z;
    result[ astro::xVelocityIndex ] = state.Velocity( ).x;
    result[ astro::yVelocityIndex ] = state.Velocity( ).y;
    result[ astro::zVelocityIndex ] = state.Velocity( ).z;
    return result;
}

//! Print value to stream.
/*!
 * Prints a specified value to stream provided, given a specified width and a filler character.
 *
 * @tparam     DataType  Type for specified value
 * @param[out] stream    Output stream
 * @param[in]  value     Specified value to print
 * @param[in]  width     Width of value printed to stream, in terms of number of characters
 *                       (default = 25)
 * @param[in]  filler    Character used to fill fixed-width (default = ' ')
 */
template< typename DataType >
inline void print( std::ostream& stream,
                   const DataType value,
                   const int width = 25,
                   const char filler = ' ' )
{
    stream << std::left << std::setw( width ) << std::setfill( filler ) << value;
}

//! Print metadata parameter to stream.
/*!
 * Prints metadata parameter to stream provided, given a specified name, value, units, and
 * delimiter.
 *
 * @tparam     DataType      Type for specified value
 * @param[out] stream        Output stream
 * @param[in]  parameterName Name for metadata parameter
 * @param[in]  value         Specified value to print
 * @param[in]  units         Units for value
 * @param[in]  delimiter     Character used to delimiter entries in stream (default = ' ')
 */
template< typename DataType >
inline void print( std::ostream& stream,
                   const std::string& parameterName,
                   const DataType value,
                   const std::string& units,
                   const char delimiter = ',' )
{
    print( stream, parameterName );
    stream << delimiter;
    print( stream, value );
    stream << delimiter;
    print( stream, units );
    stream << std::endl;
}

//! Print state history to stream.
/*!
 * Prints state history to stream provided, given a specified tate history object, a stream header,
 * and a number of digits of precision.
 *
 * @param[out] stream        Output stream
 * @param[in]  stateHistory  State history containing epochs and associated state vectors
 * @param[in]  streamHeader  A header for the output stream (file header if a file stream is
 *                           provided) (default = "")
 * @param[in]  precision     Digits of precision for state history entries printed to stream
 *                           (default = number of digits of precision for a double)
 */
void print( std::ostream& stream,
            const StateHistory stateHistory,
            const std::string& streamHeader = "",
            const int precision = std::numeric_limits< double >::digits10 );

//! Find parameter.
/* Finds parameter in config stored in JSON document. An error is thrown if the parameter cannot
 * be found. If the parameter is found, an iterator to the member in the JSON document is returned.
 *
 * @param[in] config        JSON document containing config parameters
 * @param[in] parameterName Name of parameter to find
 * @return                  Iterator to parameter retreived from JSON document
 */
inline ConfigIterator find( const rapidjson::Document& config, const std::string& parameterName )
{
    const ConfigIterator iterator = config.FindMember( parameterName.c_str( ) );
    if ( iterator == config.MemberEnd( ) )
    {
        std::cerr << "ERROR: \"" << parameterName << "\" missing from config file!" << std::endl;
        throw;
    }
    return iterator;
}

} // namespace d2d

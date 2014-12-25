/*
 * Copyright (c) 2014 K. Kumar (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <algorithm>
#include <iomanip>

#include <keplerian_toolbox.h>

#include "D2D/tools.hpp"

namespace d2d
{

//! Sample Kepler orbit.
StateHistory sampleKeplerOrbit( const Vector6& initialState,
                                const double propagationTime,
                                const int numberOfSamples,
                                const double gravitationalParameter,
                                const double initialEpoch )
{
    // Initialize state vectors.
    Vector6 state = initialState;
    Vector3 position;
    std::copy( state.begin( ), state.begin( ) + 3, position.begin( ) );
    Vector3 velocity;
    std::copy( state.begin( ) + 3, state.begin( ) + 6, velocity.begin( ) );

    // Compute size of propagation time steps.
    const double timeStep = propagationTime / static_cast< double >( numberOfSamples );

    StateHistory stateHistory;
    stateHistory[ initialEpoch ] = initialState;

    // Loop over all samples and store propagated state.
    for ( int i = 0; i < numberOfSamples; i++ )
    {
        kep_toolbox::propagate_lagrangian( position, velocity, timeStep, gravitationalParameter );
        std::copy( position.begin( ), position.begin( ) + 3, state.begin( ) );
        std::copy( velocity.begin( ), velocity.begin( ) + 3, state.begin( ) + 3 );
        stateHistory[ ( i + 1 ) * timeStep + initialEpoch ] = state;
    }

    return stateHistory;
}

//! Print state history to stream.
void print( std::ostream& stream,
            const StateHistory stateHistory,
            const std::string& streamHeader,
            const int precision )
{
        stream << streamHeader << std::endl;

        for ( StateHistory::const_iterator iteratorState = stateHistory.begin( );
              iteratorState != stateHistory.end( );
              iteratorState++ )
        {
            stream << std::setprecision( precision )
                   << iteratorState->first       << ","
                   << iteratorState->second[ 0 ] << ","
                   << iteratorState->second[ 1 ] << ","
                   << iteratorState->second[ 2 ] << ","
                   << iteratorState->second[ 3 ] << ","
                   << iteratorState->second[ 4 ] << ","
                   << iteratorState->second[ 5 ] << std::endl;
        }
}


} // namespace d2d

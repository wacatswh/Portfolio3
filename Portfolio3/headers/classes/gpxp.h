/* 	cmp3hibberb
 *	v1.15 13/3/11
 *	default ctor added to Route to support default call by Track ctor
 */



/* cmp3hibberb
 * v1.1 6/3/11
 * string field added to Route for name of Route
 * vector<string> renamed to reflect its role as names of Posn along Route
 */


/* cmp3hibberb
 * v1.0 released 21/2/2011
 */

/*
 * gpxp.hp
 *
 *  Created on: Jan 30, 2011
 *      Author: rbh
 */

#ifndef GPXP_H_
#define GPXP_H_
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

#include "../utilities/utils.h"

namespace gpxp
{
    
    class HeightProfile; //forward declaration
    
    typedef double angle;
    typedef unsigned seconds;
    typedef double metres;
    typedef double speed;
    
    
    class Posn
    {
        //v1.0
    public:
        Posn(const std::string & latSt, const std::string & lonSt,
             const std::string & eleSt = "0");
        Posn(angle lat, angle lon, angle ele = 0.0);
        metres distanceTo(const Posn &) const;
        std::string toString(bool includeElevation = true) const;
        angle lat() const;
        angle lon() const;
        angle ele() const;
        std::string eleAsString() const;
        
        
        bool isSamePlaceAs(const Posn &, const unsigned & maxSeparation) const;
        
    private:
        angle lat_;
        angle lon_;
        metres ele_;
        Posn()
        {
        }
        ; //default c'tor not available
        
    };
    
    metres distanceBetween(const Posn &, const Posn &);
    
    angle ArcInRadians( const Posn &, const Posn & );
    
    class Route
    {	//v1.0
        static const unsigned GRANULARITY = 20; // 20m minimum separation between Posn for GPXRoutes
    public:
        std::string toString() const;
        metres routeLength() const;
        metres netRouteLength() const;
        metres totalHeightGain() const;
        metres netHeightGain() const;
        angle maxUphillGradient() const;
        angle maxDownhillGradient() const;
        HeightProfile getHeightProfile() const;
        angle minLat() const;
        angle maxLat() const;
        angle minLon() const;
        angle maxLon() const;
        metres minEle() const;
        metres maxEle() const;
        unsigned timesVisited(const Posn & p) const;
        unsigned timesVisited(const std::string & nameTag) const;
        const Posn & operator[](size_t idx);
        size_t noOfRtept() const;
        std::string nameOf(const Posn & p) const;
        Posn posnOf(const std::string & nameTag) const;
        Route(std::string source, bool isFileName = false);
        bool builtOk() const;
        
    protected:
        void append(const Posn & p); // append determines whether to add its parameter to the vector, based on GRANULARITY
        void simplify(unsigned overridingGranularity = GRANULARITY);
        bool areSamePlace(const Posn & p1, const Posn &p2); // (less than GRANULARITY apart) DO NOT use nameOf()
        Route(); //only called by Track ctor
        
        /* 4 functions allowing Track ctor to set Route data
         * could equally allow protected access to private data
         */
        metres & rlen_ref();
        std::vector<Posn>   &  root_ref();
        std::string & routeName_ref();
        std::vector<std::string> & posnName_ref();		// only set if <name> set

		// declaration of refactoring functions
		std::string checkTag(string source);
		std::ostringstream processGPX(string source);

    private:
        metres rlen_; 						// set by c'tor & updated by append
        std::vector<Posn> root_;
        std::string routeName_;
        std::vector<std::string> posnName_;			// only set if <name> set
        bool ctorSucceeded_ ;
        std::string ctorReport_;
    };
    
    class Track : public Route
    {
    public:
        std::string toString() const;
        metres trackLength() const;
        metres netTrackLength() const;
        seconds totalTrackTime() const;
        seconds restingTime() const;	
        seconds travellingTime() const; 
        speed  maxSpeed() const;  
        speed  averageSpeed(bool includeRests = false); 
        speed  maxRateOfClimb() const; 
        speed  maxRateOfDescent() const;
        
        const Route & getRoute() const;  //return a const ref to the Route data in this Track
        
        Track(std::string source, bool isFileName = false);
        bool builtOk();
        
    private:
        void mergeRests();
        std::string trackName_;
        std::vector<seconds> arrived_;
        std::vector<seconds> departed_;
        bool ctorSucceeded_ ;
        std::string ctorReport_;
    };
    
} // namespace gpxp


#endif /* GPXP_H_ */

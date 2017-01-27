/* rbh 10/03/13
 *  added implementation of constructor
 *  - setting rlen_
 */


/* cmp3hibberb
 * v1.0 released 21/2/2011
 */

/*
 * gpxp.cpp
 *
 *  Created on: Jan 30, 2011
 *      Author: rbh
 */

#ifndef GPXP_CPP_
#define GPXP_CPP_

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>


#include "../headers/classes/gpxp.h"
#include "../headers/utilities/utils.h"
#include "../headers/classes/parser.h"


namespace gpxp
{
    bool implemented = true;
    using namespace gpxParser;
    using namespace xmlutils;
    
    Posn::Posn(const std::string & latSt, const std::string & lonSt,
               const std::string & eleSt)
    {
        std::string st = latSt;
        st += " ";
        st += lonSt;
        st += " ";
        st += eleSt;
        st += " ";
        std::istringstream iss(st);
        iss >> lat_ >> lon_ >> ele_;
    }
    
    Posn::Posn(angle lat, angle lon, angle ele /*= 0.0*/)
    {
        lat_ = lat;
        lon_ = lon;
        ele_ = ele;
    }
    
    metres Posn::distanceTo(const Posn & to) const
    {
        return (gpxp::distanceBetween(*this, to));
    }
    
    std::string Posn::toString(bool includeElevation) const
    {
        std::string st;
        std::ostringstream oss(st);
        oss << "lat=\"" << lat_ << "\" lon=\"" << lon_ << "\"";
        if (includeElevation)
            oss << " ele=\"" << ele_ << "\"";
        return (oss.str());
    }
    
    
    angle Posn::lat() const
    {
        return (lat_);
    }
    angle Posn::lon() const
    {
        return (lon_);
    }
    angle Posn::ele() const
    {
        return (ele_);
    }
    
    string Posn::eleAsString() const
    {
        std::string st;
        std::ostringstream oss(st);
        oss << ele_;
        return (oss.str());
    }
    
    
    //download from http://blog.julien.cayzac.name/2008/10/arc-and-distance-between-two-points-on.html
    
    /// @brief The usual PI/180 constant
    static const angle DEG_TO_RAD = 0.017453292519943295769236907684886;
    /// @brief Earth's quatratic mean radius for WGS-84
    static const angle EARTH_RADIUS_IN_METERS = 6372797.560856;
    
    /** @brief Computes the arc, in radian, between two WGS-84 positions.
     *
     * The result is equal to <code>Distance(from,to)/EARTH_RADIUS_IN_METERS</code>
     *    <code>= 2*asin(sqrt(h(d/EARTH_RADIUS_IN_METERS )))</code>
     *
     * where:<ul>
     *    <li>d is the distance in meters between 'from' and 'to' positions.</li>
     *    <li>h is the haversine function: <code>h(x)=sin(x/2)</code></li>
     * </ul>
     *
     * The haversine formula gives:
     *    <code>h(d/R) = h(from.lat-to.lat)+h(from.lon-to.lon)+cos(from.lat)*cos(to.lat)</code>
     *
     * @sa http://en.wikipedia.org/wiki/Law_of_haversines
     */
    angle ArcInRadians(const Posn& from, const Posn& to)
    {
        angle latitudeArc  = (from.lat() - to.lat()) * DEG_TO_RAD;
        angle longitudeArc = (from.lon() - to.lon()) * DEG_TO_RAD;
        angle latitudeH = sin(latitudeArc * 0.5);
        latitudeH *= latitudeH;
        angle lontitudeH = sin(longitudeArc * 0.5);
        lontitudeH *= lontitudeH;
        angle tmp = cos(from.lat()*DEG_TO_RAD) * cos(to.lat()*DEG_TO_RAD);
        return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH));
    }
    
    /** @brief Computes the distance, in meters, between two WGS-84 positions.
     *
     * The result is equal to <code>EARTH_RADIUS_IN_METERS*ArcInRadians(from,to)</code>
     *
     #include <vector>
     #include <vector>
     * @sa ArcInRadians
     */
    metres distanceBetween(const Posn& from, const Posn& to)
    {
        return EARTH_RADIUS_IN_METERS*ArcInRadians(from, to);
    }
    
    Route::Route()
    {
        ctorSucceeded_ = false;
        ctorReport_ = std::string("initial call made");
        /* actual Route object built by Track ctor
         * after Track data parsed
         */
    }
    
    Route::Route(std::string source, bool isFileName /*= false*/)
    {
        std::ostringstream oss;

        ctorSucceeded_ = false;

        if(isFileName)
        {
            std::string fileName = source;
            string content, line;
            std::ifstream fin(fileName.c_str());
            if (!fin)
            {
                oss << std::endl << "error opening constructor source file " << fileName ;
                ctorReport_ = oss.str();
                return;
            }
            oss << std::endl << "source file " << fileName << " opened okay";
            while (fin.good())
            {
                getline(fin, line);
                content += '\n';
                content += line;
            }
            oss << std::endl << "source file " << fileName << " read okay";
            source.swap(content);
        }

		ctorReport_ = checkTag(source); // check the tag of source, report kept into ctorReport_

		/*
        string temp = getWholeElement(source, "rte");
        temp = getElementValue(temp, "rte");
        source.swap(temp);
        if (source.substr(0,6)== "<name>")
        {
            string temp = getWholeElement(source, "name");
            source = source.substr(temp.length());
            routeName_ = getElementValue(temp, "name");
            oss << std::endl << "route name is " << routeName_;
        }
        while (tagExists(source, "rtept"))
        {
            temp  = getWholeElement(source, "rtept");
            source = source.substr(temp.length());
            string lt, ln, el, nm;
            lt = getAttributeValue(temp, "rtept", "lat");
            ln = getAttributeValue(temp, "rtept", "lon");
            el = getElementValue(temp, "ele");
            posnName_.push_back(tagExists(temp,"name") ? getElementValue(temp, "name"): string(""));
            root_.push_back(Posn(lt, ln, el));
        }
        oss << std::endl << root_.size() << " rtept added";
        if (root_.empty())
        {
		    oss << std::endl << "no rtept ";
            ctorReport_ = oss.str();
            return;
        }
        rlen_ = 0;
        
        std::vector<Posn>::const_iterator pci1, pci2;
        pci1 = pci2 = root_.begin();
        ++pci2;
        for ( ; pci2 != root_.end(); ++pci1, ++pci2 )
        {
            metres deltaH = pci1->distanceTo(*pci2);
            metres deltaV =  pci1->ele()  - pci2->ele() ;
            rlen_ += sqrt(pow(deltaH,2) + pow(deltaV,2));
        }
		*/
		if (tagExists(source, "gpx"))
			oss = Route::processGPX(source);
        
		// can be defined when more format is needed to process
		// if (tagExists(source, "rte"))
		//	oss = Route::processRTE(source); 

        oss << std::endl << " simplify() not implemented";
        //simplify();
        ctorReport_ = oss.str();
        ctorSucceeded_ = true;
        return;
    }
    
	/****** below are the functions declared after refactoring ******/

	// check the supported tag and return the report string
	std::string Route::checkTag(string source)
	{
		std::ostringstream osas;
		std::string osasStr;

		if (!tagExists(source, "gpx"))
		{
			osas << std::endl << "no gpx tag ";
		}

		if (!tagExists(source, "rte"))
		{
			osas << std::endl << "no rte tag ";
		}

		// add other tags inside here in future

		osasStr = osas.str();
		return osasStr;
	}

	std::ostringstream Route::processGPX(string source)
	{
		std::ostringstream oss;
		string temp = getWholeElement(source, "rte");
		temp = getElementValue(temp, "rte");
		source.swap(temp);
		if (source.substr(0, 6) == "<name>")
		{
			string temp = getWholeElement(source, "name");
			source = source.substr(temp.length());
			routeName_ = getElementValue(temp, "name");
			oss << std::endl << "route name is " << routeName_;
		}
		while (tagExists(source, "rtept"))
		{
			temp = getWholeElement(source, "rtept");
			source = source.substr(temp.length());
			string lt, ln, el, nm;
			lt = getAttributeValue(temp, "rtept", "lat");
			ln = getAttributeValue(temp, "rtept", "lon");
			el = getElementValue(temp, "ele");
			posnName_.push_back(tagExists(temp, "name") ? getElementValue(temp, "name") : string(""));
			root_.push_back(Posn(lt, ln, el));
		}
		oss << std::endl << root_.size() << " rtept added";
		if (root_.empty())
		{
			oss << std::endl << "no rtept ";
			ctorReport_ = oss.str();
			return oss;
		}
		rlen_ = 0;

		std::vector<Posn>::const_iterator pci1, pci2;
		pci1 = pci2 = root_.begin();
		++pci2;
		for (; pci2 != root_.end(); ++pci1, ++pci2)
		{
			metres deltaH = pci1->distanceTo(*pci2);
			metres deltaV = pci1->ele() - pci2->ele();
			rlen_ += sqrt(pow(deltaH, 2) + pow(deltaV, 2));
		}

		return oss;
	}

	/************** refactoring functions end here **************/

    bool Route::builtOk() const
    {
        return ctorSucceeded_;
    }
    
    std::string Route::toString() const
    {
        /* return format yet to be determined */
        // 'implemented' rbh 27/2/11
        // not really though, just delegated to ctor - use it for debugging reports
        assert(implemented);		//leave the assertion
        return (std::string(ctorReport_));
    }
    
    metres Route::routeLength() const
    {
        /*computes and returns the total length of the Route; this is
         the sum of the distances between the successive Posn of the Route*/

        assert(ctorSucceeded_ && implemented);		//leave the assertion
        return rlen_;
    }
    
    metres Route::netRouteLength() const
    {
        /* returns the distance between the first & last points on the Route*/
        assert(ctorSucceeded_ && implemented);		//leave the assertion
        const Posn & start = root_.front();
        const Posn & finish = root_.back();
        
        metres deltaH = start.distanceTo(finish);
        metres deltaV =  start.ele()  - finish.ele() ;
        return sqrt(pow(deltaH,2) + pow(deltaV,2));
    }
    
    metres Route::totalHeightGain() const
    {
        /* returns the sum of all the positive (>0) height differences between points*/
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    
    metres Route::netHeightGain() const
    {
        /* returns the difference in elevation between the first & last points on the Route
         * can be negative if netHeightLoss */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    angle Route::maxUphillGradient() const
    {
        /* returns the maximum value of gradient between successive points on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (0);
    }
    angle Route::maxDownhillGradient() const
    {
        /* returns the minimum (most negative) value of gradient between successive points on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (0);
    }
    
    /*HeightProfile Route::getHeightProfile() const
     {
     //this function requires declaration of class HeightProfile
     //no action needed until the class is declared
     }*/
    
    angle Route::minLat() const
    {
        /* returns the minimum (most negative) value of latitude i.e. most Southerly point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (90.0);
    }
    
    angle Route::maxLat() const
    {
        /* returns the maximum (most positive) value of latitude i.e. most Northerly point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (-90.0);
    }
    
    angle Route::minLon() const
    {
        /* returns the minimum (most negative) value of longitude i.e. most Westerly point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (90.0);
    }
    
    angle Route::maxLon() const
    {
        /* returns the maximum (most positive) value of longitude  i.e. most Easterly point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (-90.0);
    }
    
    metres Route::minEle() const
    {
        /* returns the minimum value of elevation i.e. lowest point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (4000);
    }
    
    metres Route::maxEle() const
    {
        /* returns the maximum value of elevation i.e. highest point on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (0);
    }
    
    unsigned Route::timesVisited(const Posn & p) const
    {
        /* returns the number of times that Posn p is visited on the Route*/
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (0);
    }
    
    unsigned Route::timesVisited(const std::string & nameTag) const
    {
        /* returns the number of times that Posn with <name>nameTag</name> is visited on the Route
         * if nameTag.empty(), return 0  */
		bool implemented = false; 	//delete this line when the function is implemented
		assert(implemented);		//leave the assertion
		return (0);
    }
    
    
    const Posn & Route::operator[]( size_t idx)
    {
        /* this function returns a not-bounds-checked access to the sequence of points
         * that make up the Route. out-of-range access should return the North Pole
         * Posn("90","0","0")
         * current 'implementation' generates a warning re: ref to temporary
         * possible solution (worth much credit) would use an auto_pointer
         * sign up (before starting) if you want a go at it*/
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (Posn("90","0","0"));
    }
    
    
    size_t Route::noOfRtept() const
    {
        /*returns the number of points (Posn) on this Route*/
        
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    
    
    std::string Route::nameOf(const Posn & thisPosn) const
    {
        /* this function determines whether the Posn thisPosn occurs anywhere on the
         * Route, and if so whether it has a name at any of the occurrences;
         * return the name if it has one, or an empty string otherwise */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (std::string("not implemented"));
    }
    
    Posn Route::posnOf(const std::string & nameTag) const
    {
        /* this function is the inverse of Route::nameOf() and searches on the Route
         * for a point named nameTag. if found, return its Posn, else the North Pole
         * Posn("90","0","0")  */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (Posn("90","0","0"));
    }
    
    void  Route::append(const Posn & posToAdd)
    {
        /*append determines whether to add its parameter to the vector, based on GRANULARITY*/
        assert(implemented);		//leave the assertion
        metres distanceSinceLast = posToAdd.distanceTo(root_.back());
        if (distanceSinceLast > GRANULARITY)
        {
            root_.push_back(posToAdd);
            rlen_ += distanceSinceLast;
        }
    }
    
    void Route::simplify(unsigned overridingGranularity /*= GRANULARITY*/)
    {
        /* this function simplifies the Route by reducing the number of points;
         * any point in the Route that differs in distance from its predecessor
         * by less than the parameter is discarded.
         * The default value GRANULARITY (class static constant) is used by c'tor
         * to limit the number of points originally, so a call with the default
         * should have no effect
         */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
    }
    
    bool Route::areSamePlace(const Posn & p1, const Posn &p2)
    {
        /* determines whether the 2 Posn parameters are less than GRANULARITY apart
         *  DO NOT use nameOf()
         *  */
        return (p1.distanceTo(p2)<GRANULARITY);
    }
    
    metres & Route::rlen_ref()
    {
        return rlen_;
    }
    
    std::vector<Posn>   &  Route::root_ref()
    {
        return  root_;
    }
    
    std::string & Route::routeName_ref()
    {
        return  routeName_;
    }
    
    std::vector<std::string> & Route::posnName_ref()
    {
        return  posnName_;
    }
    
    
    // class Track : public Route
    
    
    Track::Track(std::string source, bool isFileName /*= false*/)//:Route(source, isFileName)
    {
        std::ostringstream oss;
        ctorSucceeded_ = false;
        if (isFileName)
        {
            std::string fileName = source;
            string content, line;
            std::ifstream fin(fileName.c_str());
            if (!fin)
            {
                oss << std::endl << "error opening constructor source file " << fileName ;
                ctorReport_ = oss.str();
                return;
            }
            oss << std::endl << "source file " << fileName << " opened okay";
            while (fin.good())
            {
                getline(fin, line);
                content += '\n';
                content += line;
            }
            oss << std::endl << "source file " << fileName << " read okay";
            source.swap(content);
        }
        if (!tagExists(source,"trk"))
        {
            oss << std::endl << "no trk tag ";
            ctorReport_ = oss.str();
            return;
        }
        string temp = getWholeElement(source, "trk");
        temp = getElementValue(temp, "trk");
        source.swap(temp);
        if (source.substr(0,6)== "<name>")
        {
            string temp = getWholeElement(source, "name");
            source = source.substr(temp.length());
            trackName_ = getElementValue(temp, "name");
            routeName_ref() = std::string("track ") + trackName_;
            oss << std::endl << "track name is " << trackName_;
        }
        std::string mergedTrkSegs, trkseg, segname;
        while (tagExists(source, "trkseg"))
        {
            trkseg = getWholeElement(source, "trkseg");
            source = source.substr(trkseg.length());
            trkseg = getElementValue(trkseg, "trkseg"); // lose tags
            segname = getWholeElement(trkseg, "name");
            trkseg = trkseg.substr(segname.length());
            mergedTrkSegs += trkseg;
        }
        if (!mergedTrkSegs.empty())
            mergedTrkSegs.swap(source);
        if (!tagExists(source,"trkpt"))
        {
            oss << std::endl << "no trkpt tag ";
            ctorReport_ = oss.str();
            return;
        }
        temp  = getWholeElement(source, "trkpt");
        source = source.substr(temp.length());
        string lt, ln, el, nm, thyme;
        lt = getAttributeValue(temp, "trkpt", "lat");
        ln = getAttributeValue(temp, "trkpt", "lon");
        el = getElementValue(temp, "ele");
        thyme = getElementValue(temp, "time");
        posnName_ref().push_back(tagExists(temp,"name") ? getElementValue(temp, "name"): string(""));
        Posn fromPoint(lt, ln, el);
        arrived_.push_back(timeFromString(thyme));//slot[0] is absolute time (track start) all others relative to this
        departed_.push_back(0);				//slot[0] is zero - departed_ are all relative - so last is net track time
        root_ref().push_back(fromPoint);
        oss << std::endl << "Track start Posn added : " <<  fromPoint.toString();
        while (tagExists(source, "trkpt"))
        {
            temp  = getWholeElement(source, "trkpt");
            source = source.substr(temp.length());
            lt = getAttributeValue(temp, "trkpt", "lat");
            ln = getAttributeValue(temp, "trkpt", "lon");
            el = getElementValue(temp, "ele");
            thyme = getElementValue(temp, "time");
            Posn toPoint(lt, ln, el);
            if (areSamePlace(toPoint, fromPoint))
            {
                departed_.back() = timeFromString(thyme);//will continue to be overwritten until 'new' point reached
                oss << std::endl << "Track Posn ignored : " <<  fromPoint.toString();
            }
            else							//update times/positions/vectors
            {
                posnName_ref().push_back(tagExists(temp,"name") ? getElementValue(temp, "name"): string(""));
                root_ref().push_back(toPoint);
                seconds elapsed = timeFromString(thyme) - arrived_.front();
                arrived_.push_back(elapsed);
                departed_.push_back(elapsed);
                rlen_ref() += fromPoint.distanceTo(toPoint);
                fromPoint = toPoint;
                oss << std::endl << "Track Posn added : " <<  fromPoint.toString();
                oss << " at time " <<  timeAsString(departed_.back());
            }
        }
        oss << std::endl << " rlen_ set in Route";
        ctorReport_ = oss.str();
        ctorSucceeded_ = true;
        return;
    }
    
    bool Track::builtOk()
    {
        return ctorSucceeded_;
    }
    
    std::string Track::toString() const
    {
        // 'implemented' rbh 13/3/11
        // not really though, just delegated to ctor - use it for debugging reports
        assert(implemented);		//leave the assertion
        return (std::string(ctorReport_));
    }
    
    metres Track::trackLength() const
    {
        /* essentially routeLength() which this should call
         * the difficulty is accessing the Base class component of this derived class
         * see PointylandTrack for ideas  and possibly Track::getRoute()*/
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    
    metres Track::netTrackLength() const
    {
        /* same as Track::trackLength()
         * see PointylandTrack for ideas */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    
    seconds Track::totalTrackTime() const
    {
        /* total elapsed time between start & finish of the Track */
        //bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    seconds Track::restingTime() const
    {
        /* total elapsed time between start & finish of the Track that is spent not moving
         * one of the 3 functions should use
         * totaltrackTime = restingTime + travellingTime */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    seconds Track::travellingTime() const
    {
        /* total elapsed time between start & finish of the Track that is spent moving
         * one of the 3 functions should use
         * totaltrackTime = restingTime + travellingTime */
        //bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0);
    }
    speed Track::maxSpeed() const
    {
        /* what is the greatest speed over a section of the track (between 2 Posn) */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    speed Track::averageSpeed(bool includeRests /*= false*/)
    {
        /* what is the average speed over the track the parameter determines whether
         * the divisor is the totalTime or the travellingTime */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    speed Track::maxRateOfClimb() const
    {
        /* what is the greatest rate of climb over a section of the track (between 2 Posn) */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    speed Track::maxRateOfDescent() const
    {
        /* what is the greatest rate of descent over a section of the track (between 2 Posn) */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (0.0);
    }
    
    const Route & Track::getRoute() const
    {
        /* return a const ref to the Route data in this Track
         * see PointylandTrack for an example.
         * check it works!
         */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
        return (*this);
    }
    
    void Track::mergeRests()
    {
        /*this should be superfluous as 'not moved far enough to count as a new point'
         * would have the same effect.
         */
        bool implemented = false; 	//delete this line when the function is implemented
        assert(implemented);		//leave the assertion
    }
    
} // namespace gpxp


#endif /* GPXP_CPP_ */

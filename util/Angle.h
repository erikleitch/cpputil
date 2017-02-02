#ifndef GCP_UTIL_ANGLE_H
#define GCP_UTIL_ANGLE_H

/**
 * @file Angle.h
 * 
 * Tagged: Tue Aug 10 13:49:00 PDT 2004
 * 
 * @author Erik Leitch
 */
#include <cmath>
#include <string>
#include <iostream>

#include "gcp/util/ConformableQuantity.h"

namespace gcp {
  namespace util {
    
    class Angle : public ConformableQuantity {
    public:
      
      class Radians {};
      class Degrees {};
      class ArcSec {};
      class ArcMinutes {};
      class MilliArcSec {};

      /**
       * Public constructors with no value initialization
       */
      Angle();

      /**
       * Unambiguous contructor for degree strings
       */
      Angle(std::string degrees, bool modulo=false);

      /**
       * Constructors with unit specifications
       */
      Angle(const Radians& units,     double radians, bool modulo=false);
      Angle(const Degrees& units,     double degrees, bool modulo=false);
      Angle(const Degrees& units,     std::string degrees, bool modulo=false);
      Angle(const MilliArcSec& units, double mas,     bool modulo=false);
      Angle(const ArcSec& units,      double as,      bool modulo=false);
      Angle(const ArcMinutes& units,  double am,      bool modulo=false);

      Angle(const Angle& angle);
      Angle(Angle& angle);
      void operator=(const Angle& angle);
      void operator=(Angle& angle);

      bool operator!=(const Angle& angle);
      bool operator!=(Angle& angle);

      /**
       * Destructor.
       */
      virtual ~Angle();
      
      /**
       * Set the angle
       */
      void setRadians(double radians);
      void setDegrees(double degrees);
      void setDegrees(double degrees, double arcmin, double arcsec);
      void setArcMinutes(double am);
      void setMas(double mas);
      void setArcSec(double as);
      void setDegrees(std::string degrees);
      void setVal(double val, std::string units);
      virtual void setVal(std::string val);

      /**
       * Add to the angle
       */
      virtual void addRadians(double radians);
      virtual void addDegrees(double degrees);
      virtual void addDegrees(std::string degrees);

      /**
       * Get the angle
       */
      inline double radians() {
	return val_;
      }

      inline double degrees() {
	return val_ * degPerRad_;
      }

      inline double arcsec() {
	return val_ * arcSecPerRad_;
      }

      inline double arcmin() {
	return val_ * arcMinPerRad_;
      }

      inline double mas() {
	return val_ * masPerRad_;
      }

      std::string strDegrees();

      /**
       * Allows cout << Angle
       */
      friend std::ostream& operator<<(std::ostream& os, const Angle& angle);
      friend std::ostream& operator<<(std::ostream& os, Angle& angle);

      /** 
       * Add two Angles
       */
      Angle operator+(Angle& angle);
      Angle operator+(const Angle& angle);
      
      /** 
       * Add two Angles
       */
      void operator+=(Angle& angle);
      void operator+=(const Angle& angle);

      /** 
       * Subtract two Angles
       */
      Angle operator-(Angle& angle);

      /** 
       * Divide an angle by an integer
       */
      void operator/=(unsigned uval);

      /** 
       * Divide an angle by an integer
       */
      Angle operator/(unsigned uval);
      Angle operator/(double dval);
      double operator/(Angle angle);

      /** 
       * Multiply an angle by an integer
       */
      void operator*=(unsigned uval);

      /** 
       * Multiply an angle by an integer
       */
      Angle operator*(unsigned uval);

      // Multiply an angle by a double

      Angle operator*(double dval);

      /** 
       * Compare two Angles
       */
      bool operator>(Angle& angle);
      bool operator>(const Angle& angle);
      bool operator>=(Angle& angle);
      bool operator<(Angle& angle);
      bool operator<(const Angle& angle);
      bool operator<=(Angle& angle);

      //------------------------------------------------------------
      // Public utility methods

      /**
       * Convert a sexagesimal string to a double representation
       */
      static double sexagesimalToDouble(std::string valStr);

      /**
       * Convert a double to a sexagesimal string
       */
      static std::string doubleToSexagesimal(double val);

      static const double pi_;
      static const double twoPi_;

      static const double degPerRad_;
      static const double arcSecPerDegree_;
      static const double masPerDegree_;
      static const double arcSecPerRad_;
      static const double arcMinPerRad_;
      static const double masPerRad_;

      void initialize();

    protected:

      bool modulo_;    // True if this angle should be manipulated
		       // modulo twoPi_

    }; // End class Angle

    std::ostream& operator<<(std::ostream& os, const Angle& angle);
    std::ostream& operator<<(std::ostream& os, Angle& angle);

    Angle operator*(unsigned uval, const Angle& angle);
    Angle operator*(unsigned uval, Angle& angle);
    
  } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_ANGLE_H

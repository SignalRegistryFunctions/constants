#ifndef SRFUN_CONSTANTS
#define SRFUN_CONSTANTS

// Standard C libraries

// External libraries

#ifndef FLOAT
#define FLOAT double
#endif

#include <string.h>

#ifndef FUNCTION
#define FUNCTION "anonymous"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif

typedef struct
{

  /* Fundamental Constants (SI) */
  FLOAT speed_of_light;      // m/s
  FLOAT vacuum_permeability; // H/m
  FLOAT vacuum_permittivity; // F/m
  FLOAT planck_h;            // J·s
  FLOAT planck_hbar;         // J·s
  FLOAT avogadro;            // mol^-1
  FLOAT faraday;             // C/mol
  FLOAT boltzmann;           // J/K
  FLOAT molar_gas;           // J/(mol·K)
  FLOAT standard_gas_volume; // m^3/mol
  FLOAT stefan_boltzmann;    // W/(m^2·K^4)
  FLOAT gauss;               // unitless (T to SI)

  /* Astronomy & Astrophysics */
  FLOAT astronomical_unit;      // m
  FLOAT gravitational_constant; // m^3/(kg·s^2)
  FLOAT light_year;             // m
  FLOAT parsec;                 // m
  FLOAT grav_accel;             // m/s^2
  FLOAT solar_mass;             // kg

  /* Atomic & Nuclear Physics */
  FLOAT electron_charge;          // C
  FLOAT electron_volt;            // J
  FLOAT unified_atomic_mass;      // kg
  FLOAT mass_electron;            // kg
  FLOAT mass_muon;                // kg
  FLOAT mass_proton;              // kg
  FLOAT mass_neutron;             // kg
  FLOAT fine_structure;           // unitless
  FLOAT rydberg;                  // J
  FLOAT bohr_radius;              // m
  FLOAT angstrom;                 // m
  FLOAT barn;                     // m^2
  FLOAT bohr_magneton;            // J/T
  FLOAT nuclear_magneton;         // J/T
  FLOAT electron_magnetic_moment; // J/T
  FLOAT proton_magnetic_moment;   // J/T
  FLOAT thomson_cross_section;    // m^2
  FLOAT debye;                    // C·m

  /* Time */
  FLOAT minute; // s
  FLOAT hour;   // s
  FLOAT day;    // s
  FLOAT week;   // s

  /* Imperial & Length */
  FLOAT inch; // m
  FLOAT foot; // m
  FLOAT yard; // m
  FLOAT mile; // m
  FLOAT mil;  // m

  /* Speed & Nautical */
  FLOAT km_per_hour;    // m/s
  FLOAT miles_per_hour; // m/s
  FLOAT nautical_mile;  // m
  FLOAT fathom;         // m
  FLOAT knot;           // m/s

  /* Printer Units */
  FLOAT point;    // m
  FLOAT texpoint; // m

  /* Volume, Area, Mass */
  FLOAT micron;          // m
  FLOAT hectare;         // m^2
  FLOAT acre;            // m^2
  FLOAT liter;           // m^3
  FLOAT us_gallon;       // m^3
  FLOAT canadian_gallon; // m^3
  FLOAT uk_gallon;       // m^3
  FLOAT quart;           // m^3
  FLOAT pint;            // m^3

  /* Mass and Weight */
  FLOAT pound_mass;      // kg
  FLOAT ounce_mass;      // kg
  FLOAT ton;             // kg
  FLOAT metric_ton;      // kg
  FLOAT uk_ton;          // kg
  FLOAT troy_ounce;      // kg
  FLOAT carat;           // kg
  FLOAT gram_force;      // N
  FLOAT pound_force;     // N
  FLOAT kilopound_force; // N
  FLOAT poundal;         // N

  /* Pressure */
  FLOAT bar;              // Pa
  FLOAT std_atmosphere;   // Pa
  FLOAT torr;             // Pa
  FLOAT meter_of_mercury; // Pa
  FLOAT inch_of_mercury;  // Pa
  FLOAT inch_of_water;    // Pa
  FLOAT psi;              // Pa

  /* Viscosity */
  FLOAT poise;  // Pa·s
  FLOAT stokes; // m^2/s

  /* Light & Illumination */
  FLOAT stilb;       // cd/m^2
  FLOAT lumen;       // lm
  FLOAT lux;         // lx
  FLOAT phot;        // phot
  FLOAT footcandle;  // fc
  FLOAT lambert;     // L
  FLOAT footlambert; // fL

  /* Radioactivity */
  FLOAT curie;    // Bq
  FLOAT roentgen; // C/kg
  FLOAT rad;      // Gy

} Constants;

extern const Constants constants;

#endif

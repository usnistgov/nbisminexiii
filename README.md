NBIS MINEX III
==============

The [National Institute Standards and Technology Biometric Image Software
(NBIS)][1] [Minutia Interoperability Exchange (MINEX) III][2] wraps the
fingerprint template generator and fingerprint template matcher provided as part
of [NBIS][1] in the [MINEX III API][3]. NIST then runs this library through the
[MINEX III][2] test, enabling comparison to other closed-source algorithms.

More information about the wrapped algorithms (`mindtct` and `bozorth3`) can be
found in the [NBIS documentation].

MINEX III report cards are available for this library:

 * [Template Generator Report]
 * [Template Matcher Report]

Communication
-------------
Changes in this repository should be limited to the wrapping of the MINEX III API. Core algorithm changes should be addressed in the NBIS codebase.

If you found a bug and can provide steps to reliably reproduce it, or if you
have a feature request, please [open an issue]. Other questions may be addressed
to the [NIST MINEX team](mailto:minex@nist.gov).

License
-------
This repository contains code from other NIST projects (BioMDI, MINEX III, and NBIS). The items in this and the other NIST repositories are released in the public domain. See the
[LICENSE] for details.

[1]: https://www.nist.gov/services-resources/software/nist-biometric-image-software-nbis
[2]: https://www.nist.gov/programs-projects/minutiae-interoperability-exchange-minex-overview
[3]: https://github.com/usnistgov/minex/blob/master/minexiii/testplan.pdf
[NBIS documentation]: https://doi.org/10.6028/NIST.IR.7391
[open an issue]: https://github.com/usnistgov/nbisminexiii/issues
[LICENSE]: https://github.com/usnistgov/nbisminexiii/blob/master/LICENSE.md
[Template Generator Report]: https://pages.nist.gov/minex/results/reportcards/pdf/minexiii/nbisminexiii+5002_generator_report.pdf
[Template Matcher Report]: https://pages.nist.gov/minex/results/reportcards/pdf/minexiii/nbisminexiii+5002_matcher_report.pdf
#ifndef EXPORT_FILE_RELATIONS_H
#define EXPORT_FILE_RELATIONS_H

#include "ExportFileAbstractBase.h"
#include "ExportPointsIntervalUnits.h"
#include "ExportValuesOrdinal.h"
#include <QStringList>
#include <QVector>

class Document;
class DocumentModelCoords;
class DocumentModelExport;
class QTextStream;
class Transformation;

/// Strategy class for exporting to a file. This strategy is external to the Document class so that class is simpler.
class ExportFileRelations : public ExportFileAbstractBase
{
public:
  /// Single constructor.
  ExportFileRelations();

  /// Export Document points according to the settings. The DocumentModelExport inside the Document is ignored so
  /// DlgSettingsExport can supply its own DocumentModelExport when previewing what would be exported.
  void exportToFile (const DocumentModelExport &modelExportOverride,
                     const Document &document,
                     const Transformation &transformation,
                     QTextStream &str) const;

private:
  void exportAllPerLineXThetaValuesMerged (const DocumentModelExport &modelExportOverride,
                                           const Document &document,
                                           const QStringList &curvesIncluded,
                                           const QString &delimiter,
                                           const Transformation &transformation,
                                           QTextStream &str) const;
  void exportOnePerLineXThetaValuesMerged (const DocumentModelExport &modelExportOverride,
                                           const Document &document,
                                           const QStringList &curvesIncluded,
                                           const QString &delimiter,
                                           const Transformation &transformation,
                                           QTextStream &str) const;
  void initializeXThetaYRadiusValues (const QStringList &curvesIncluded,
                                      QVector<QVector<QString*> > &xThetaYRadiusValues) const;
  QPointF linearlyInterpolate (const Points &points,
                               double ordinal,
                               const Transformation &transformation) const;
  void loadXThetaYRadiusValues (const DocumentModelExport &modelExportOverride,
                                const Document &document,
                                const QStringList &curvesIncluded,
                                const Transformation &transformation,
                                QVector<QVector<QString*> > &xThetaYRadiusValues) const;
  void loadXThetaYRadiusValuesForCurveInterpolatedSmooth (const DocumentModelCoords &modelCoords,
                                                          const Points &points,
                                                          const ExportValuesOrdinal &ordinals,
                                                          QVector<QString*> &xThetaValues,
                                                          QVector<QString*> &yRadiusValues,
                                                          const Transformation &transformation) const;
  void loadXThetaYRadiusValuesForCurveInterpolatedStraight (const DocumentModelCoords &modelCoords,
                                                            const Points &points,
                                                            const ExportValuesOrdinal &ordinals,
                                                            QVector<QString*> &xThetaValues,
                                                            QVector<QString*> &yRadiusValues,
                                                            const Transformation &transformation) const;
  void loadXThetaYRadiusValuesForCurveRaw (const DocumentModelCoords &modelCoords,
                                           const Points &points,
                                           QVector<QString*> &xThetaValues,
                                           QVector<QString*> &yRadiusValues,
                                           const Transformation &transformation) const;
  int maxColumnSizeAllocation (const DocumentModelExport &modelExportOverride,
                               const Document &document,
                               const Transformation &transformation,
                               const QStringList &curvesIncluded) const;
  ExportValuesOrdinal ordinalsAtIntervals (double pointsIntervalRelations,
                                           ExportPointsIntervalUnits pointsIntervalUnits,
                                           const Transformation &transformation,
                                           const Points &points) const;
  ExportValuesOrdinal ordinalsAtIntervalsGraph (double pointsIntervalRelations,
                                                const Transformation &transformation,
                                                const Points &points) const;
  ExportValuesOrdinal ordinalsAtIntervalsScreen (double pointsIntervalRelations,
                                                 const Points &points) const;

  /// Output 2D array with alternating x/theta and y/radius columns
  void outputXThetaYRadiusValues (const DocumentModelExport &modelExport,
                                  const QStringList &curvesIncluded,
                                  QVector<QVector<QString*> > &xThetaYRadiusValues,
                                  const QString &delimiter,
                                  QTextStream &str) const;
};

#endif // EXPORT_FILE_RELATIONS_H
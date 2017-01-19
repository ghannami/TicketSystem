/*!
 *##########################################################
 * \class   TrackingDetails
 * \package
 * \author  Aziz Ghannami
 * \date    16.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef TRACKINGDETAILS_H
#define TRACKINGDETAILS_H

#include <QWidget>

namespace Ui {
class TrackingDetails;
}

class TrackingData;

class TrackingDetails : public QWidget
{
    Q_OBJECT

public:
    explicit TrackingDetails(QWidget *parent = 0);
    ~TrackingDetails();

    TrackingData *trackingData() const;
    void setTrackingData(TrackingData *trackingData);

private:
    Ui::TrackingDetails *ui;
    TrackingData *m_trackingData;
};

#endif // TRACKINGDETAILS_H

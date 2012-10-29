#ifndef ELEMENTS_ACTION_GAUGE_H
#define ELEMENTS_ACTION_GAUGE_H

#define ACTION_GAUGE_MAX (100.0f)
#define ACTION_GAUGE_FULL (99.99f)

void createActionGauge(float posX, float posY);
void setActionGaugePosition(float x, float y);
void setActionGaugeValue(float value);
void deleteActionGauge();

#endif /* ELEMENTS_ACTION_GAUGE_H */

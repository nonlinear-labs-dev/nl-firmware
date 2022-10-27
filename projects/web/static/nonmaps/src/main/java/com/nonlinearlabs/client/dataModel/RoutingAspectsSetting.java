package com.nonlinearlabs.client.dataModel;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;

public class RoutingAspectsSetting {
    
    public static int numAspects = 5;
    public static int numRoutings = 12;

    public RoutingAspectsSetting() {
        m_data = new boolean[numRoutings][numAspects];
    }

    static RoutingAspectsSetting fromString(String s) {
        RoutingAspectsSetting r = new RoutingAspectsSetting();
        
        String splitRoutingsCommaSeperatedList[] = s.split("\n");

        for(int routing = 0; routing < numRoutings; routing++) { 
            String[] splitOnesAndZeros = splitRoutingsCommaSeperatedList[routing].split(",");
            
            boolean[] row = new boolean[numAspects];
            for(int aspect = 0; aspect < numAspects; aspect++) {
                row[aspect] = splitOnesAndZeros[aspect] == "1";
            }
            r.m_data[routing] = row;
        }

        int sum = 0;
        for(boolean[] row: r.m_data) {
            for(boolean entry: row) {
                sum += entry ? 1 : 0;
            }
        }

        return r;
    }

    @Override
    public String toString() {
        String ret = new String();
        for(boolean routing[] : m_data)
        {
          for(boolean value : routing)
          {
            ret += value ? "1," : "0,";
          }
          ret += "\n";
        }
        return ret;
    }

    private int idToIndex(int id)
    {
        switch(id)
        {
            case 284:
            case 404:
               return 6;
            case 289:
            case 405:
                return 7;
            case 254:
            case 398:
                return 0;
            case 259:
            case 399:
                return 1;
            case 264:
            case 400:
                return 2;
            case 269:
            case 401:
                return 3;
            case 274:
            case 402:
                return 4;
            case 279:
            case 403:
                return 5;
            case 409:
            case 410:
                return 10;
            case 411:
            case 412:
                return 11;
        }
        return 0;
    }

    public boolean isLocalEnabled(PhysicalControlParameterModel param)
    {
        final int LOCAL_INDEX = 4;
        return m_data[idToIndex(param.id.getNumber())][LOCAL_INDEX];
    }

    public boolean isLocalEnabled(int parameterId)
    {
        final int LOCAL_INDEX = 4;
        return m_data[idToIndex(parameterId)][LOCAL_INDEX];
    }

    public boolean[][] m_data;    
}
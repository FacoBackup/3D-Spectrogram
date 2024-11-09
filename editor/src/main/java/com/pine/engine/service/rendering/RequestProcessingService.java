package com.pine.engine.service.rendering;

import com.pine.impl.Loggable;
import com.pine.engine.injection.PBean;
import com.pine.engine.service.request.AbstractRequest;

import java.util.LinkedList;
import java.util.List;

@PBean
public class RequestProcessingService implements Loggable {
    private static final int MAX_HISTORY = 15;

    private final List<AbstractRequest> requests = new LinkedList<>();

    public void addRequest(AbstractRequest request) {
        if(requests.size() >= MAX_HISTORY){
            requests.removeFirst();
        }
        requests.add(request);

        try{
            request.run();
        }catch (Exception e){
            getLogger().error(e.getMessage(), e);
        }
    }
}

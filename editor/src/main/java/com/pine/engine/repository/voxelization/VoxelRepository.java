package com.pine.engine.repository.voxelization;

import com.pine.impl.Icons;
import com.pine.engine.injection.PBean;
import com.pine.engine.inspection.Inspectable;
import com.pine.engine.inspection.MutableField;
import org.joml.Vector3f;

@PBean
public class VoxelRepository extends Inspectable {
    public transient int[] voxels = null;


    @MutableField(label = "Object scale")
    public int gridScale = 100;

    @MutableField(label = "Scene scale", min = 0)
    public int gridResolution = 20;

    @MutableField(label = "Center")
    public Vector3f center = new Vector3f(0);

    @MutableField(label = "Max depth", min = 1)
    public int maxDepth = 10;

    @MutableField(label = "Voxelization step size", min = 0, max = 1)
    public float voxelizationStepSize = .1f;

    @MutableField(label = "Random colors")
    public boolean randomColors = true;

    @MutableField(label = "Show ray search count")
    public boolean showRaySearchCount;

    @MutableField(label = "Show ray test count")
    public boolean showRayTestCount;

    @MutableField(label = "Show grid")
    public boolean showGrid = true;

    @Override
    public String getTitle() {
        return "Voxelized scene";
    }

    @Override
    public String getIcon() {
        return Icons.apps;
    }

    public int getVoxelCount() {
        if (voxels != null) {
            return voxels.length;
        }
        return 0;
    }
}

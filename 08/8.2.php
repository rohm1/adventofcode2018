<?php

$fileName = __DIR__ . '/8.txt';

class Entity
{
    /** @var int */
    public $name;

    /** @var Entity[] */
    public $childs = [];

    /** @var int[] */
    public $metadata = [];

    /** @var Entity */
    public $parent;
}

class Parser
{
    /** @var array */
    protected $licenseParts;

    /** @var int */
    protected $crtIndex;

    /** @var int */
    protected $licenseLength;

    /** @var Entity */
    protected $root;

    /** @var Entity[] */
    protected $entities = [];

    /** @var Entity */
    protected $crtEntity;

    /** @var int */
    protected $metadataSum;

    /**
     * @param string $license
     */
    public function __construct(string $license)
    {
        $this->licenseParts = explode(' ', trim($license));
        $this->licenseLength = count($this->licenseParts);
    }

    /**
     * @return Parser
     */
    public function parse(): self
    {
        $this->crtIndex = 0;
        $this->metadataSum = 0;
        $this->parseEntity();

        return $this;
    }

    protected function parseEntity(): void
    {
        $entity = new Entity();

        if ($this->crtEntity) {
            $entity->name = count($this->entities) + 1;
            $entity->parent = $this->crtEntity;
            $this->crtEntity->childs[] = $entity;
        } else {
            $entity->name = 1;
            $this->root = $entity;
        }

        $this->crtEntity = $entity;
        $this->entities[$entity->name] = $entity;

        $childs = (int) $this->licenseParts[$this->crtIndex++];
        $metadata = (int) $this->licenseParts[$this->crtIndex++];

        for ($i = 0; $i < $childs; $i++) {
            $this->parseEntity();
        }

        for ($i = 0; $i < $metadata; $i++) {
            $m = (int) $this->licenseParts[$this->crtIndex++];
            $this->metadataSum += $m;
            $entity->metadata[] = $m;
        }

        $this->crtEntity = $entity->parent;
    }

    /**
     * @return int
     */
    public function getSum(): int
    {
        return $this->getNodeSum($this->root);
    }

    /**
     * @param Entity $entity
     * @return int
     */
    protected function getNodeSum(Entity $entity): int
    {
        if (count($entity->childs) === 0) {
            return array_sum($entity->metadata);
        }

        $sum = 0;
        foreach ($entity->metadata as $metadata) {
            if (isset($entity->childs[$metadata - 1])) {
                $sum += $this->getNodeSum($entity->childs[$metadata - 1]);
            }
        }

        return $sum;
    }
}

var_dump(
    (new Parser(file_get_contents($fileName)))
        ->parse()
        ->getSum()
);
